/*! \file system.c
*
*  \brief System clock setups.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "system.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/
void SYSTEM_clockConfig(void);

/****************************** Functions Definition ***********************************/
/**
* \name     SYSTEM_init
* \brief    Initialize the system
*
* \param    None
* \retval   None
*/
void SYSTEM_init( void )
{
	/* initialize the clocks */
	SYSTEM_clockConfig();
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SYSTEM_clockConfig(void)
{

}
