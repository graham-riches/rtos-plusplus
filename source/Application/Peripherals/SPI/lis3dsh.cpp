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

/******************************* Global Variables **************************************/
static HAL::OutputPin
    accelerometer_chip_select(GPIOE, HAL::Pins::pin_3, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_up, HAL::OutputMode::push_pull);
static HAL::AlternateModePin accelerometer_sck(
    GPIOA, HAL::Pins::pin_5, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5);
static HAL::AlternateModePin accelerometer_miso(
    GPIOA, HAL::Pins::pin_6, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5);
static HAL::AlternateModePin accelerometer_mosi(
    GPIOA, HAL::Pins::pin_7, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5);

LIS3DSH accelerometer(SPI1, accelerometer_chip_select);

static std::map<LIS3DSHResolution, uint16_t> acceleration_conversion_map = {{LIS3DSHResolution::resolution_2g, 0x4000},
                                                                            {LIS3DSHResolution::resolution_4g, 0x2000},
                                                                            {LIS3DSHResolution::resolution_6g, 0x1554},
                                                                            {LIS3DSHResolution::resolution_8g, 0x1000},
                                                                            {LIS3DSHResolution::resolution_16g, 0x0800}};


/**
 * \brief Construct a new LIS3DSH::LIS3DSH object
 * 
 * \param spi_peripheral_address 
 * \param chip_select 
 */
LIS3DSH::LIS3DSH(SPI_TypeDef* spi_peripheral_address, HAL::OutputPin chip_select)
    : HAL::SPIInterrupt(spi_peripheral_address, chip_select) {
    conversion_factor = acceleration_conversion_map[LIS3DSHResolution::resolution_2g];
}

/**
 * \brief read the value of a register
 * 
 * \param reg the register to read
 * \retval uint8_t value of the register
 */
uint8_t LIS3DSH::read_register(LIS3DSHRegisters reg) {
    uint16_t read_command = static_cast<uint8_t>(reg) | device_read;
    send(reinterpret_cast<uint8_t*>(&read_command), sizeof(read_command));

    // block until the register result returns
    uint8_t data = 0;
    while ( !rx_buffer.empty() ) {
        auto maybe_data = rx_buffer.pop_front();
        data = (maybe_data.has_value()) ? maybe_data.value() : 0;
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

    send(write_command, sizeof(write_command));
}

/**
 * \brief setup a LIS3DSH accelerometer
 */
void LIS3DSH::initialize(void) {
    using namespace HAL;

    // Enable the peripheral clock
    reset_control_clock.set_apb_clock(APB2Clocks::spi_1, true);

    // Setup the SPI control register settings
    write_control_register(SPIControlRegister1::master_select, 0x01);
    write_control_register(SPIControlRegister1::clock_polarity, 0x01);
    write_control_register(SPIControlRegister1::clock_phase, 0x01);
    write_control_register(SPIControlRegister2::slave_select_output_enable, 0x01);
    set_baudrate(SPIBaudratePrescaler::prescaler_16);  //!< 84MHz / 16 = 5.25 MHz

    // Register the SPI interrupt
    interrupt_manager.register_callback(InterruptName::spi_1, this, static_cast<uint8_t>(InterruptType::spi_interrupt), PreemptionPriority::level_2);

    // Enable the interrupt
    write_control_register(SPIControlRegister2::receive_interrupt_enable, 0x01);

    // Enable the peripheral
    write_control_register(SPIControlRegister1::spi_enable, 0x01);

    // Register the external interrupts
    register_external_interrupt(EXTIPort::gpio_port_e, Pins::pin_0, EXTITrigger::rising);
    interrupt_manager.register_callback(InterruptName::exti_0, this, static_cast<uint8_t>(InterruptType::external_interrupt_1), PreemptionPriority::level_2);

    // Setup the accelerometer speed and setup the data ready interrupt
    set_data_rate(LIS3DSHDataRate::sample_100Hz);
    set_resolution(LIS3DSHResolution::resolution_2g);
    write_register(LIS3DSHRegisters::control_register_3, interrupt_1_data_ready);
    write_register(LIS3DSHRegisters::control_register_6, enable_multibyte_read);
}

/**
 * \brief set the sample rate of the accelerometer
 * 
 * \param rate the rate
 */
void LIS3DSH::set_data_rate(LIS3DSHDataRate rate) {
    write_register(LIS3DSHRegisters::control_register_4, static_cast<uint8_t>(rate));
}

/**
 * \brief set the accelerometer resolution
 * 
 * \param resolution the resolution in units of g's
 */
void LIS3DSH::set_resolution(LIS3DSHResolution resolution) {
    write_register(LIS3DSHRegisters::control_register_5, static_cast<uint8_t>(resolution));
    conversion_factor = acceleration_conversion_map[resolution];
}

/**
 * \brief test function for the accelerometer
 * \retval return the result of the who am I register
 */
uint8_t LIS3DSH::self_test(void) {
    uint8_t self_test = read_register(LIS3DSHRegisters::who_am_i);
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
            spi_irq_handler();
            break;

        case InterruptType::external_interrupt_1:
            exti_0_irq_handler();
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
    chip_select.set(false);

    while ( !tx_buffer.empty() ) {
        // Wait for the transmit buffer to clear
        while ( read_status_register(HAL::SPIStatusRegister::transmit_data_empty) == false ) {
        }

        // Put data outgoing into the data register
        auto maybe_data = tx_buffer.pop_front();
        if ( maybe_data.has_value() ) {
            peripheral->DR = maybe_data.value();
        }

        // Wait for the receive buffer to clear
        while ( read_status_register(HAL::SPIStatusRegister::receive_data_available) == false ) {
        }

        // Receive data into the rx buffer
        rx_buffer.push_back(static_cast<uint8_t>(peripheral->DR));
    }
    
    write_control_register(HAL::SPIControlRegister2::transmit_interrupt_enable, 0x00);    
    chip_select.set(true);
}

/**
 * \brief interrupt handler for exti 0 interrupt
 *        This is configured to be used as accelerometer data ready
 * 
 */
void LIS3DSH::exti_0_irq_handler(void) {
    rx_buffer.flush();

    // Read XYZ data - Note: the registers are chained so it can be done in a single call
    uint8_t read_data[read_register_data_size] = {0};
    read_data[0] = static_cast<uint8_t>(LIS3DSHRegisters::output_x) | device_read;
    send(read_data, sizeof(read_data));

    // Flush the first byte, which is read back when the address + read byte is first sent
    rx_buffer.pop_front();
    
    auto convert_data = [this](uint8_t low, uint8_t high) -> float { return static_cast<float>(static_cast<int16_t>(low | (high << 8))) / conversion_factor; };

//!< TODO: banking on these being successfull reads
//!< TODO: this is broken!!
#if 0
    float accel_x = convert_data(rx_buffer.pop().value(), rx_buffer.pop().value());
    float accel_y = convert_data(rx_buffer.pop().value(), rx_buffer.pop().value());
    float accel_z = convert_data(rx_buffer.pop().value(), rx_buffer.pop().value());

    x_data.push(accel_x);
    y_data.push(accel_y);
    z_data.push(accel_z);
#endif

    /* clear the interrupt pending bit */
    HAL::clear_external_interrupt_pending(HAL::EXTILine::line_0);
}
