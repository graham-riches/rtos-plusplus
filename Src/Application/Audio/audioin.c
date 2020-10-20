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
//#include "adc.h"
#include "arm_math.h"

/*********************************** Consts ********************************************/
#define AUDIOIN_BUFFER_SAMPLES     (4096ul)   //!< total "dual-bank" circular buffer size
#define AUDIOIN_HALFBUFFER_SAMPLES (2048ul)   //!< size of each DMA half bank
#define AUDIOIN_FFT_SIZE           (1024ul)   //!< FFT size is half of each banks size
#define AUDIOIN_SAMPLE_RATE        (2000.0f) //!< sample rate of the input signal

/************************************ Types ********************************************/
typedef enum
{
    AUDIOIN_BANK0,
    AUDIOIN_BANK1,
    AUDIOIN_TOTAL_BANKS,
}AUDIOIN_bank_t;

/**
 * \brief handler structure for audio input
 */
typedef struct
{
    uint16_t       buffer[AUDIOIN_BUFFER_SAMPLES];
    uint16_t       *bufferBank0;
    uint16_t       *bufferBank1;
    AUDIOIN_bank_t activeBank;
} AUDIOIN_handler_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static AUDIOIN_handler_t input;
static float rawData[AUDIOIN_HALFBUFFER_SAMPLES] = {0};
static float fftData[AUDIOIN_HALFBUFFER_SAMPLES] = {0};
static float fftMagnitude[AUDIOIN_FFT_SIZE] = {0};
/* setup FFT parameters */
/* TODO: this will need to be modified based on the operating mode of the system once
 * I start adding in more effects :)
 */
arm_rfft_fast_instance_f32 rfft;
float peakMagnitude;
uint32_t peakMagnitudeIndex;
float signalFrequency;

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

    /* set ping-pong buffering up using the single data buffer in the handler struct.
       DMA is in circular mode, so this can be used as a dual bank
       buffer with the half/full callbacks signaling the ends of each bank.
     */
    input.bufferBank0 = input.buffer;
    input.bufferBank1 = input.bufferBank0 + AUDIOIN_HALFBUFFER_SAMPLES;
    input.activeBank = AUDIOIN_BANK0;

    /* setup the FFT function from the DSP */
    arm_rfft_fast_init_f32( &rfft, AUDIOIN_FFT_SIZE );

    /* start the ADC DMA sampling */
    //ADC_startDMA( ADC_AUDIO_INPUT, (uint32_t *)&input.buffer, AUDIOIN_BUFFER_SAMPLES );
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
    /* FFT processes the data in-place so set the pointer to the appropriate buffer */
    uint16_t *dataPtr = (input.activeBank == AUDIOIN_BANK0) ? input.bufferBank0 : input.bufferBank1;

    /* do some prescaling */
    arm_q15_to_float( (q15_t *)dataPtr, rawData, AUDIOIN_HALFBUFFER_SAMPLES );
    arm_rfft_fast_f32( &rfft, rawData, fftData, 0 );

    /* calculate the magnitude of each bin */
    arm_cmplx_mag_f32( fftData, fftMagnitude, AUDIOIN_FFT_SIZE );

    /* get the maximum FFT magnitude to determine the frequency component of the input signal */
    fftMagnitude[0] = 0; //!< zero the DC component as we don't care about it
    arm_max_f32( fftMagnitude, AUDIOIN_FFT_SIZE, &peakMagnitude, &peakMagnitudeIndex );
    signalFrequency = (float)peakMagnitudeIndex * ((float)AUDIOIN_SAMPLE_RATE/(float)(AUDIOIN_FFT_SIZE));

    /* flop the buffer for next time */
    input.activeBank = (input.activeBank == AUDIOIN_BANK0) ? AUDIOIN_BANK1 : AUDIOIN_BANK0;
    return;
}
