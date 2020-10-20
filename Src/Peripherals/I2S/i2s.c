/*! \file i2s.c
*
*  \brief HAL I2S drivers for on-board audio chip.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "i2s.h"


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static I2S_handler_t i2sHandlersArray[I2S_MAX_DEVICES];

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     I2S_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void I2S_init( void )
{

    I2S_HandleTypeDef *hi2s;
    for ( uint8_t device = 0; device < I2S_MAX_DEVICES; device++ )
    {
        hi2s = &i2sHandlersArray[device].handler;

        switch ( device )
        {
            case I2S_AUDIO_DEVICE:
                hi2s->Instance = SPI3;
                hi2s->Init.Mode = I2S_MODE_MASTER_TX;
                hi2s->Init.Standard = I2S_STANDARD_PHILIPS;
                hi2s->Init.DataFormat = I2S_DATAFORMAT_16B;
                hi2s->Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
                hi2s->Init.AudioFreq = I2S_AUDIOFREQ_96K;
                hi2s->Init.CPOL = I2S_CPOL_LOW;
                hi2s->Init.ClockSource = I2S_CLOCK_PLL;
                hi2s->Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
                break;

            default:
                break;
        }

        /* initialize the peripheral */
        if ( HAL_I2S_Init(hi2s) != HAL_OK )
        {
            /* TODO: error handler */
        }
    }
    return;
}
