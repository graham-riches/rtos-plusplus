/*! \file adc.c
*
*  \brief Setup and initialization of ADC peripherals.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "adc.h"
#include "board.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static ADC_handler_t adcHandlersArray[ADC_TOTAL_DEVICES] = {0};

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     ADC_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void ADC_init( void )
{
   ADC_HandleTypeDef *adc;
   ADC_ChannelConfTypeDef *channel;
   GPIO_InitTypeDef gpio;

   /* common gpio parameters */
   gpio.Mode = GPIO_MODE_ANALOG;  //!< configure the GPIO pins analog
   gpio.Pull = GPIO_NOPULL;

   for ( uint8_t device = 0; device < ADC_TOTAL_DEVICES; device++ )
   {
       adc = &adcHandlersArray[device].handler;
       channel = &adcHandlersArray[device].channel;

       switch ( device )
       {
           case ADC_AUDIO_INPUT:
               /* enable the peripheral clock */
               __HAL_RCC_ADC1_CLK_ENABLE();

               /* setup gpio pin */
               gpio.Pin = AUDIO_IN_PIN;

               /* init the GPIO */
               HAL_GPIO_Init( AUDIO_IN_PORT, &gpio );

               /* setup the audio input ADC parameters */
               adc->Instance = AUDIO_IN_ADC;
               adc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
               adc->Init.Resolution = ADC_RESOLUTION_12B;
               adc->Init.ScanConvMode = DISABLE;
               adc->Init.ContinuousConvMode = ENABLE;
               adc->Init.DiscontinuousConvMode = DISABLE;
               adc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
               adc->Init.NbrOfConversion = 1;
               adc->Init.DMAContinuousRequests = DISABLE;
               adc->Init.EOCSelection = ADC_EOC_SEQ_CONV;

               if ( HAL_ADC_Init( adc ) != HAL_OK )
               {
                   /* TODO: error handler */
               }

               /* setup the ADC channel parameters */
               channel->Channel = AUDIO_IN_ADC_CHANNEL;
               channel->Rank = 1;
               channel->SamplingTime = ADC_SAMPLETIME_144CYCLES;
               if ( HAL_ADC_ConfigChannel( adc, channel ) != HAL_OK )
               {
                   /* TODO: error handler */
               }

               /* start the ADC */
               HAL_ADC_Start( adc );
               break;

           default:
               break;
       }
   }
}


/**
 * \name ADC_getValue
 * \brief get a measurement value from a specific ADC device
 * \param device the device to measure from
 * \retval output value of the peripheral
 */
uint32_t ADC_getValue( ADC_devices_t device )
{
    if ( device >= ADC_TOTAL_DEVICES )
    {
        return 0;
    }
    ADC_HandleTypeDef *adc = &adcHandlersArray[device].handler;
    HAL_ADC_PollForConversion( adc, HAL_MAX_DELAY );
    uint32_t value = HAL_ADC_GetValue( adc );
    return value;
}

