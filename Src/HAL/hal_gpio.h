/*! \file hal_gpio.h
*
*  \brief HAL library implementation of STM32F4 cortex M4 GPIO functionality
*
*
*  \author Graham Riches
*/

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

/********************************** Includes *******************************************/

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
      enum class PinMode : unsigned { input = 0x00, output = 0x01, alternate = 0x02, analog = 0x03 };


      /**
       * \brief GPIO pin output modes if the pin is configured as an output
       *    push_pull : configure the output in push pull mode
       *    open_drain: configure the output in open drain mode
       */
      enum class OutputMode : unsigned { push_pull = 0x00, open_drain = 0x01 };


      /**
       * \brief GPIO switching speeds (fairly self-explanatory)       
       */
      enum class Speed : unsigned { low = 0x00, medium = 0x01, high = 0x02, very_high = 0x03 };


      /**
       * \brief GPIO pull mode. 
       *    none : no pull up or pull down activated
       *    pull_up : internal pull up activated
       *    pull_down : internal pull down activated
       *    reserved : saved my manufactured      
       */
      enum class PullMode : unsigned { none = 0x00, pull_up = 0x01, pull_down = 0x02, reserved = 0x03 };


      /**
       * \brief specification for GPIO pin alternate mode, which are used for other peripherals
       * 
       */
      enum class AlternateMode : unsigned { af0 = 0, af1 = 1, af2 = 2, af3 = 3, af4 = 4, af5 = 5,
                                          af6 = 6, af7 = 7, af8 = 8, af9 = 9, af10 = 10, af11 = 11,
                                          af12 = 12, af13 = 13, af14 = 14, af15 = 15};


      /**
       * \brief enumeration of GPIO pins that can be OR'd together
       * 
       */
      enum class Pins : unsigned { pin_0 = 0, pin_1 = 1, pin_2 = 2, pin_3 = 3, pin_4 = 4, pin_5 = 5,
                                 pin_6 = 6, pin_7 = 7, pin_8 = 8, pin_9 = 9, pin_10 = 10, pin_11 = 11,
                                 pin_12 = 12, pin_13 = 13, pin_14 = 14, pin_15 = 15, all = 16 };


      /**
       * \brief configuration structure for a pin/pins       
       */
      typedef struct
      {
         Pins pins;
         PinMode mode;
         OutputMode output_mode;
         Speed speed;
         PullMode pull_mode;
         AlternateMode alternate_mode;         
      } PinConfiguration;

      /*********************************** Macros ********************************************/


      /****************************** Function Declarations ************************************/
      void initialize_pin( PinConfiguration configuration );
   };
}


#endif /* __HAL_GPIO_H__ */
