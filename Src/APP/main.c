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
#include "debug.h"
#include "event.h"
#include "accelerometer.h"

/*********************************** Consts ********************************************/
#define COMMAND_BUFFER_SIZE 256

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/*********************************** Local Variables ********************************************/
static char msgIn[COMMAND_BUFFER_SIZE] = {0};
static uint16_t bytesReceived = 0;

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

    /* initialize the event flags manager */
    EVENT_init();

    /* initialize the accelerometer */
    ACCEL_init();

    /* light the LEDs */
    GPIO_setLED( GPIO_LED_GREEN, GPIO_LED_ON );
    GPIO_setLED( GPIO_LED_ORANGE, GPIO_LED_ON );
    GPIO_setLED( GPIO_LED_RED, GPIO_LED_ON );
    GPIO_setLED( GPIO_LED_BLUE, GPIO_LED_ON );

    /* main single-threaded function */
    /* TODO: clean this into a more compact structure with callbacks/ function pointers */
    while (1)
    {
        /* check for events */
        if ( mainEventFlags )
        {
            /* got data on the debug port, so read it */
            if ( EVENT_get(&mainEventFlags, EVENT_USART_DEBUG_RX) )
            {
                bytesReceived += USART_recv( USART_DEBUG, (uint8_t *)&msgIn[bytesReceived], COMMAND_BUFFER_SIZE - bytesReceived );
                if ( msgIn[bytesReceived - 1] == '\n' )
                {
                    /* NULL terminate the message and send it to the CLI */
                    msgIn[bytesReceived - 1] = '\0';
                    CLI_executeCommand( msgIn );

                    /* clear the buffer and reset */
                    bytesReceived = 0;
                    memset( msgIn, 0, COMMAND_BUFFER_SIZE );
                }
            }

            /* got an accelerometer event */
            if ( EVENT_get(&mainEventFlags, EVENT_ACCEL_BUFF_FULL) )
            {
                ACCEL_processDataBuffer();
            }
        }
    }
    return 0;
}
