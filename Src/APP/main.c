/*! \file main.c
*
*  \brief Main application code
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <string.h>
#include "board.h"
#include "hal.h"
#include "usart.h"
#include "common.h"
#include "command_line.h"

/*********************************** Consts ********************************************/
#define COMMAND_BUFFER_SIZE 256

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/*********************************** Local Variables ********************************************/
static char msgIn[COMMAND_BUFFER_SIZE] = {0};
static uint16_t bytesReceived = 0;
static char *in = msgIn;
static uint16_t count;

/**
  * \brief  Main application function
  * \retval int
  */
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* configure the project specific HAL drivers */
    HAL_moduleInit();

    /* light the LEDs */
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_12, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_13, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_14, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_15, GPIO_PIN_SET );

    /* main single-threaded function */
    while (1)
    {
        bytesReceived += (uint16_t)USART_recv( USART_DEBUG, (uint8_t *)&msgIn[bytesReceived], (uint16_t)(COMMAND_BUFFER_SIZE - bytesReceived) );

        /* scan through the buffer and check for a message termination */
        if ( bytesReceived )
        {
            in = msgIn;
            count = 0;
            while ( count < COMMAND_BUFFER_SIZE )
            {
                if ( *in == '\n' )
                {
                    /* NULL terminate the string */
                    *in = '\0';
                    *++in = '0';

                    /* send it to the command line interface module */
                    CLI_executeCommand( msgIn );

                    /* clear the buffer and break out of the loop */
                    bytesReceived = 0;
                    memset( msgIn, 0, COMMAND_BUFFER_SIZE );
                    break;
                }
                count++;
                in++;
            }

        }

        HAL_Delay( 500 );
    }
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_12, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_13, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_14, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_15, GPIO_PIN_RESET );
    return 0;
}
