/*! \file i2c.c
*
*  \brief HAL I2C drivers.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "i2c.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static I2C_handler_t i2cHandlersArray[I2C_MAX_DEVICES];

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     I2C_init
* \brief    Initialize the I2C peripheral
*
* \param    None
* \retval   None
*/
void I2C_init( void )
{
    I2C_HandleTypeDef *hi2c;

    for ( uint8_t device = 0; device < I2C_MAX_DEVICES; device++ )
    {
        hi2c = &i2cHandlersArray[device].handler;

        switch ( device )
        {
            case I2C_DEFAULT_DEVICE:
                hi2c->Instance = I2C1;
                hi2c->Init.ClockSpeed = 100000;
                hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
                hi2c->Init.OwnAddress1 = 0;
                hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
                hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
                hi2c->Init.OwnAddress2 = 0;
                hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
                hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
                break;

            default:
               break;
        }

        if ( HAL_I2C_Init(hi2c) != HAL_OK )
        {
            /* TODO: implement callback error handler */
        }
    }
    return;
}
