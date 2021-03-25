/*! \file accelerometer.c
*
*  \brief LIS302 Accelerometer peripheral.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "lis3dsh.h"
#include "board.h"
#include "hal_exti.h"
#include "hal_interrupt.h"
#include "hal_rcc.h"
#include <map>


/*********************************** Consts ********************************************/
constexpr uint8_t buffer_size = 128;
constexpr uint8_t data_fifo_depth = 16;
constexpr uint8_t register_write_size = 2;
constexpr uint8_t device_read = 0x80;
constexpr uint8_t device_write = 0x00;
constexpr uint8_t interrupt_1_data_ready = 0b11001000;
constexpr uint8_t enable_multibyte_read = 0b00010000;
constexpr uint8_t read_register_data_size = 7;

/************************************ Types ********************************************/
/**
 * \brief configuration of interrupts for the accelerometer
 */
enum class InterruptType : unsigned {
    spi_interrupt = 0,
    external_interrupt_1,
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
/* create the pins */
static HAL::OutputPin
    accelerometer_chip_select(GPIOE, HAL::Pins::pin_3, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_up, HAL::OutputMode::push_pull);
static HAL::AlternateModePin accelerometer_sck(
    GPIOA, HAL::Pins::pin_5, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5);
static HAL::AlternateModePin accelerometer_miso(
    GPIOA, HAL::Pins::pin_6, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5);
static HAL::AlternateModePin accelerometer_mosi(
    GPIOA, HAL::Pins::pin_7, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5);

LIS3DSH accelerometer(SPI1, accelerometer_chip_select, buffer_size, buffer_size, data_fifo_depth);

static std::map<LIS3DSHResolution, uint16_t> acceleration_conversion_map = {{LIS3DSHResolution::resolution_2g, 0x4000},
                                                                            {LIS3DSHResolution::resolution_4g, 0x2000},
                                                                            {LIS3DSHResolution::resolution_6g, 0x1554},
                                                                            {LIS3DSHResolution::resolution_8g, 0x1000},
                                                                            {LIS3DSHResolution::resolution_16g, 0x0800}};

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new LIS3DSH::LIS3DSH object
 * 
 * \param spi_peripheral_address the memory mapped peripheral address
 * \param chip_select the chip select pin
 * \param tx_size size of the tx buffer
 * \param rx_size size of the rx_buffer
 * \param data_fifo_depth how many measurements to buffer
 */
LIS3DSH::LIS3DSH(SPI_TypeDef* spi_peripheral_address, HAL::OutputPin chip_select, size_t tx_size, size_t rx_size, size_t data_fifo_depth)
    : HAL::SPIInterrupt(spi_peripheral_address, chip_select, tx_size, rx_size)
    , x_data(data_fifo_depth)
    , y_data(data_fifo_depth)
    , z_data(data_fifo_depth) {
    /* default initialize the conversion factor to +/- 2g */
    this->conversion_factor = acceleration_conversion_map[LIS3DSHResolution::resolution_2g];
}

/**
 * \brief read the value of a register
 * 
 * \param reg the register to read
 * \retval uint8_t value of the register
 */
uint8_t LIS3DSH::read_register(LIS3DSHRegisters reg) {
    uint16_t read_command = static_cast<uint8_t>(reg) | device_read;
    this->send(reinterpret_cast<uint8_t*>(&read_command), sizeof(read_command));

    /* block until the register result returns */
    uint8_t data;
    while ( !this->rx_buffer.is_empty() ) {
        data = this->rx_buffer.get();
    }

    return data;
}

/**
 * \brief write a value to a register
 * 
 * \param reg the register to write to
 * \param value to write
 */
void LIS3DSH::write_register(LIS3DSHRegisters reg, uint8_t value) {
    uint8_t write_command[register_write_size];
    write_command[0] = (static_cast<uint8_t>(reg) | device_write);
    write_command[1] = value;

    this->send(write_command, sizeof(write_command));
}

/**
 * \brief setup a LIS3DSH accelerometer
 */
void LIS3DSH::initialize(void) {
    using namespace HAL;

    /* enable the peripheral clock */
    reset_control_clock.set_apb_clock(APB2Clocks::spi_1, true);

    /* setup the SPI control register settings */
    this->write_control_register(SPIControlRegister1::master_select, 0x01);
    this->write_control_register(SPIControlRegister1::clock_polarity, 0x01);
    this->write_control_register(SPIControlRegister1::clock_phase, 0x01);
    this->write_control_register(SPIControlRegister2::slave_select_output_enable, 0x01);
    this->set_baudrate(SPIBaudratePrescaler::prescaler_16);  //!< 84MHz / 16 = 5.25 MHz

    /* register the SPI interrupt */
    interrupt_manager.register_callback(InterruptName::spi_1, this, static_cast<uint8_t>(InterruptType::spi_interrupt), 5);

    /* enable the interrupt */
    this->write_control_register(SPIControlRegister2::receive_interrupt_enable, 0x01);

    /* enable the peripheral */
    this->write_control_register(SPIControlRegister1::spi_enable, 0x01);

    /* register the external interrupts */
    register_external_interrupt(EXTIPort::gpio_port_e, Pins::pin_0, EXTITrigger::rising);
    interrupt_manager.register_callback(InterruptName::exti_0, this, static_cast<uint8_t>(InterruptType::external_interrupt_1), 16);

    /* setup the accelerometer speed and setup the data ready interrupt */
    this->set_data_rate(LIS3DSHDataRate::sample_100Hz);
    this->set_resolution(LIS3DSHResolution::resolution_2g);
    this->write_register(LIS3DSHRegisters::control_register_3, interrupt_1_data_ready);
    this->write_register(LIS3DSHRegisters::control_register_6, enable_multibyte_read);
}

/**
 * \brief set the sample rate of the accelerometer
 * 
 * \param rate the rate
 */
void LIS3DSH::set_data_rate(LIS3DSHDataRate rate) {
    this->write_register(LIS3DSHRegisters::control_register_4, static_cast<uint8_t>(rate));
}

/**
 * \brief set the accelerometer resolution
 * 
 * \param resolution the resolution in units of g's
 */
void LIS3DSH::set_resolution(LIS3DSHResolution resolution) {
    this->write_register(LIS3DSHRegisters::control_register_5, static_cast<uint8_t>(resolution));
    this->conversion_factor = acceleration_conversion_map[resolution];
}

/**
 * \brief test function for the accelerometer
 * \retval return the result of the who am I register
 */
uint8_t LIS3DSH::self_test(void) {
    uint8_t self_test = this->read_register(LIS3DSHRegisters::who_am_i);
    return self_test;
}

/**
 * \brief main interrupt handler
 * 
 * \param type the type of interrupt to handle
 */
void LIS3DSH::irq_handler(uint8_t type) {
    InterruptType interrupt = static_cast<InterruptType>(type);

    switch ( interrupt ) {
        case InterruptType::spi_interrupt:
            this->spi_irq_handler();
            break;

        case InterruptType::external_interrupt_1:
            this->exti_0_irq_handler();
            break;

        default:
            break;
    }
}

/**
 * \brief interrupt handler for SPI interrupts
 * \note because there are multiple interrupts attached to the LIS3HD object,
 *       this here is a copy of the SPI interrupt
 */
void LIS3DSH::spi_irq_handler(void) {
    /* enable the chip select */
    this->chip_select.set(false);

    while ( !this->tx_buffer.is_empty() ) {
        /* wait for the transmit buffer to clear */
        while ( this->read_status_register(HAL::SPIStatusRegister::transmit_data_empty) == false ) {
        }

        /* put data outgoing into the data register */
        this->peripheral->DR = this->tx_buffer.get();

        /* wait for the receive buffer to clear */
        while ( this->read_status_register(HAL::SPIStatusRegister::receive_data_available) == false ) {
        }

        /* receive data into the rx buffer */
        this->rx_buffer.put(static_cast<uint8_t>(this->peripheral->DR));
    }

    /* disable the interrupt */
    this->write_control_register(HAL::SPIControlRegister2::transmit_interrupt_enable, 0x00);

    /* disable the chip select */
    this->chip_select.set(true);
}

/**
 * \brief interrupt handler for exti 0 interrupt
 *        This is configured to be used as accelerometer data ready
 * 
 */
void LIS3DSH::exti_0_irq_handler(void) {
    this->rx_buffer.flush();

    /* read XYZ data - Note: the registers are chained so it can be done in a single burst */
    uint8_t read_data[read_register_data_size] = {0};
    read_data[0] = static_cast<uint8_t>(LIS3DSHRegisters::output_x) | device_read;
    this->send(read_data, sizeof(read_data));

    /* flush the first byte, which is read back when the address + read byte is first sent */
    this->rx_buffer.get();

    /* update the data values */
    auto convert_data = [this](uint8_t low, uint8_t high) -> float {
        return static_cast<float>(static_cast<int16_t>(low | (high << 8))) / this->conversion_factor;
    };
    float accel_x = convert_data(this->rx_buffer.get(), this->rx_buffer.get());
    float accel_y = convert_data(this->rx_buffer.get(), this->rx_buffer.get());
    float accel_z = convert_data(this->rx_buffer.get(), this->rx_buffer.get());

    this->x_data.put(accel_x);
    this->y_data.put(accel_y);
    this->z_data.put(accel_z);

    /* clear the interrupt pending bit */
    HAL::clear_external_interrupt_pending(HAL::EXTILine::line_0);
}
