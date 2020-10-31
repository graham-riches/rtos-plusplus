/*! \file hal_usart.h
*
*  \brief hal_usart module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __HAL_USART_H__
#define __HAL_USART_H__

/********************************** Includes *******************************************/
#include "hal_bitwise_operators.h"
#include "hal_interrupt.h"
#include "hal_rcc.h"
#include "ring_buffer.h"
#include "stm32f4xx.h"
#include <stdint.h>

namespace HAL
{
namespace USART
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief bit offsets for USART status register
 */
enum class StatusRegister : unsigned
{
   parity_error = 0,
   framing_error = 1,
   noise_detected = 2,
   overrun_error = 3,
   idle_detected = 4,
   receive_data_available = 5,
   transmission_complete = 6,
   transmit_data_empty = 7,
   line_break_detected = 8,
   cts_flag = 9
};

/**
 * \brief bit offsets for USART control register one
 */
enum class ControlRegister1 : unsigned
{
   send_break = 0,
   receiver_wakeup = 1,
   receiver_enable = 2,
   transmitter_enable = 3,
   idle_interrupt_enable = 4,
   receive_interrupt_enable = 5,
   transmit_complete_interrupt_enable = 6,
   transmit_interrupt_enable = 7,
   parity_error_interrupt_enable = 8,
   parity_selection = 9,
   parity_control_enable = 10,
   wake_method = 11,
   word_length = 12,
   usart_enable = 13,
   oversampling_enable = 15
};

/**
 * \brief bit offsets for USART control register two
 */
enum class ControlRegister2 : unsigned
{
   address = 0,
   line_break_detection_length = 5,
   line_break_interrupt_enable = 6,
   last_bit_clock_pulse = 8,
   clock_phase = 9,
   clock_polarity = 10,
   clock_enable = 11,
   stop_bits = 12,
   line_mode_enable = 14,
};

/**
 * \brief bit offsets for USART control register three
 */
enum class ControlRegister3 : unsigned
{
   error_interrupt_enable = 0,
   irda_mode_enable = 1,
   irda_low_power_mode = 2,
   half_duplex_enable = 3,
   smartcard_nack_enable = 4,
   smartcard_enable = 5,
   dma_receive_enable = 6,
   dma_transmit_enable = 7,
   rts_enable = 8,
   cts_enable = 9,
   cts_interrupt_enable = 10,
   onebit_mode_enable = 11,
};

/**
 * \brief base class for uart peripherals
 */
class USARTBase
{
   protected:
   USART_TypeDef *peripheral;

   public:
   USARTBase( USART_TypeDef *usart )
   {
      this->peripheral = usart;
   }
   virtual void initialize( ){ };
};

/**
 * \brief base class for interrupt driven usart peripherals. This is meant to be inherited
 *        to create an interrupt driven object
 */
class USARTInterrupt : protected USARTBase, public HAL::Interrupt::InterruptPeripheral
{
   protected:
   RingBuffer<uint8_t> tx_buffer;
   RingBuffer<uint8_t> rx_buffer;

   public:
   USARTInterrupt( USART_TypeDef *usart, size_t tx_size, size_t rx_size )
      : USARTBase( usart )
      , tx_buffer( tx_size )
      , rx_buffer( rx_size )
   { }

   void irq_handler( uint8_t type );
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/
bool read_status_register( USART_TypeDef *usart, StatusRegister reg );
void write_control_register( USART_TypeDef *usart, ControlRegister1 reg, uint8_t value );
void write_control_register( USART_TypeDef *usart, ControlRegister2 reg, uint8_t value );
void write_control_register( USART_TypeDef *usart, ControlRegister3 reg, uint8_t value );
void set_baudrate( USART_TypeDef *usart, HAL::ResetControlClock::Clocks clock, uint32_t baudrate );

};  // namespace USART
};  // namespace HAL

#endif /* __HAL_USART_H__ */
