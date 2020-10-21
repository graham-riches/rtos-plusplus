/*! \file dma.c
*
*  \brief DMA module initialization.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "dma.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     DMA_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void DMA_init( void )
{
    /* Initialize the module here*/
    __DMA1_CLK_ENABLE();
    __DMA2_CLK_ENABLE();
}
