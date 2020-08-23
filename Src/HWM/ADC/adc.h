/*! \file adc.h
*
*  \brief adc module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"
#include "stm32f4xx_hal.h"

/*********************************** Consts ********************************************/
#define ADC_DMA_BUFFER_SIZE 64

/************************************ Types ********************************************/
/**
 * \brief handler structure for ADC peripherals
 */
typedef struct
{
    ADC_HandleTypeDef      handler;                     //!< ADC HAL handler
    ADC_ChannelConfTypeDef channel;                     //!< ADC Channel config
    DMA_HandleTypeDef      dma;                         //!< DMA handler
} ADC_handler_t;


/**
 * \brief enumeration of ADC peripherals
 */
typedef enum
{
    ADC_AUDIO_INPUT = 0,
    ADC_TOTAL_DEVICES,
} ADC_devices_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void     ADC_init( void );
uint32_t ADC_getValue( ADC_devices_t device );
void     ADC_startDMA( ADC_devices_t device, uint32_t *buffer, uint32_t size );


#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
