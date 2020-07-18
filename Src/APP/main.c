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
#include "gpio.h"

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
    GPIO_setLED( GPIO_LED_GREEN, GPIO_LED_ON );
    GPIO_setLED( GPIO_LED_ORANGE, GPIO_LED_ON );
    GPIO_setLED( GPIO_LED_RED, GPIO_LED_ON );
    GPIO_setLED( GPIO_LED_BLUE, GPIO_LED_ON );

    /* main single-threaded function */
    while (1)
    {
        bytesReceived += (uint16_t)USART_recv( USART_DEBUG, (uint8_t *)&msgIn[bytesReceived], (uint16_t)(COMMAND_BUFFER_SIZE - bytesReceived) );

        /* scan through the buffer and check for a message termination */
        if ( bytesReceived )
        {
            in = msgIn;
            count = 0;
            while ( count < COMMAND_BUFFER_SIZE - 1 )
            {
                if ( *in == '\n' )
                {
                    /* NULL terminate the string */
                    *in = '\0';
                    *++in = '0';

                    /* send it to the command line interface module */
                    CLI_executeCommand( msgIn );
                    /* clear the buffer and reset */
                    bytesReceived = 0;
                    memset( msgIn, 0, COMMAND_BUFFER_SIZE );
                    break;
                }
                count++;
                in++;
            }
            if ( bytesReceived >= COMMAND_BUFFER_SIZE - 1 )
            {
                bytesReceived = 0;
                memset( msgIn, 0, COMMAND_BUFFER_SIZE );
            }
        }

        HAL_Delay( 500 );
    }
    return 0;
}
