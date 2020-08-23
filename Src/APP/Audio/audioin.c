/*! \file audioin.c
*
*  \brief audio input sampling and processing via ADC.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "audioin.h"
#include "string.h"
#include "debug.h"
#include "adc.h"

/*********************************** Consts ********************************************/
#define AUDIOIN_BUFFER_SAMPLES     (64ul)
#define AUDIOIN_HALFBUFFER_SAMPLES ((uint32_t)AUDIOIN_BUFFER_SAMPLES/2)

/************************************ Types ********************************************/
/**
 * \brief handler structure for audio input
 */
typedef struct
{
    uint16_t buffer[AUDIOIN_BUFFER_SAMPLES];
    uint16_t *currentDataPtr;
} AUDIOIN_handler_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static AUDIOIN_handler_t input;

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     AUDIOIN_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void AUDIOIN_init( void )
{
    /* Initialize the module here*/
    memset( &input, 0, sizeof(AUDIOIN_handler_t) );
    /* start the ADC DMA sampling */
    ADC_startDMA( ADC_AUDIO_INPUT, (uint32_t *)&input.buffer, AUDIOIN_BUFFER_SAMPLES );
}


/**
* \name     AUDIOIN_processDataBuffer
* \brief    handle a buffer full event by processing the data
*
* \param    None
* \retval   None
*/
void AUDIOIN_processDataBuffer( void )
{
    /* placeholder function */
    DEBUG_print( "AUDIOIN buffer full event" );
}
