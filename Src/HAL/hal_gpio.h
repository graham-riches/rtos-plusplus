/*! \file hal_gpio.h
*
*  \brief HAL++ library implementation of STM32F4 cortex M4 GPIO functionality
*
*
*  \author Graham Riches
*/

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

/********************************** Includes *******************************************/
#include "hal_bitwise_operators.h"
#include "stm32f4xx.h"
#include <stdint.h>

namespace HAL
{
namespace GPIO
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief GPIO pin modes
 *    input : pin is configured as an input pin
 *    output : pin is configured as an output pin
 *    alternate : pin is configured was an alternate (using another peripheral)
 *    analog : pin is configured as an analog pin
 */
enum class PinMode : unsigned
{
   input = 0x00,
   output = 0x01,
   alternate = 0x02,
   analog = 0x03
};

/**
 * \brief GPIO pin output modes if the pin is configured as an output
 *    push_pull : configure the output in push pull mode
 *    open_drain: configure the output in open drain mode
 */
enum class OutputMode : unsigned
{
   push_pull = 0x00,
   open_drain = 0x01
};

/**
 * \brief GPIO switching speeds (fairly self-explanatory)       
 */
enum class Speed : unsigned
{
   low = 0x00,
   medium = 0x01,
   high = 0x02,
   very_high = 0x03
};

/**
 * \brief GPIO pull mode. 
 *    none : no pull up or pull down activated
 *    pull_up : internal pull up activated
 *    pull_down : internal pull down activated
 *    reserved : saved my manufactured      
 */
enum class PullMode : unsigned
{
   none = 0x00,
   pull_up = 0x01,
   pull_down = 0x02,
   reserved = 0x03
};

/**
 * \brief specification for GPIO pin alternate mode, which are used for other peripherals
 */
enum class AlternateMode : unsigned
{
   af0 = 0,
   af1 = 1,
   af2 = 2,
   af3 = 3,
   af4 = 4,
   af5 = 5,
   af6 = 6,
   af7 = 7,
   af8 = 8,
   af9 = 9,
   af10 = 10,
   af11 = 11,
   af12 = 12,
   af13 = 13,
   af14 = 14,
   af15 = 15
};

/**
 * \brief enumeration of GPIO pins that can be OR'd together
 */
enum class Pins : unsigned
{
   pin_0 = 0x0001,
   pin_1 = 0x0002,
   pin_2 = 0x0004,
   pin_3 = 0x0008,
   pin_4 = 0x0010,
   pin_5 = 0x0020,
   pin_6 = 0x0040,
   pin_7 = 0x0080,
   pin_8 = 0x0100,
   pin_9 = 0x0200,
   pin_10 = 0x0400,
   pin_11 = 0x0800,
   pin_12 = 0x1000,
   pin_13 = 0x2000,
   pin_14 = 0x4000,
   pin_15 = 0x8000,
   all = 0xFFFF
};

/**
 * \brief class type for a GPIO output pin
 */
class OutputPin
{
   private:
   GPIO_TypeDef *bank;
   Pins pin;
   Speed speed;
   PullMode pull_mode;

   public:
   OutputPin() {} //!< default constructor
   OutputPin( GPIO_TypeDef *bank, Pins pin, Speed speed, PullMode pull_mode, OutputMode output_mode );
   void initialize( GPIO_TypeDef *bank, Pins pin, Speed speed, PullMode pull_mode, OutputMode output_mode );

   void set( bool high );
};

/*********************************** Macros ********************************************/

/****************************** Function Declarations ************************************/
void initialize_pin( GPIO_TypeDef *bank, Pins pin, Speed speed, PullMode pull_mode, OutputMode output_mode );
void set_alternate_mode( GPIO_TypeDef *bank, Pins pins, AlternateMode alternate );

};  // namespace GPIO
};  // namespace HAL

#endif /* __HAL_GPIO_H__ */
