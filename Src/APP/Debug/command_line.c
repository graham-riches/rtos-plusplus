/*! \file command_line.c
 *	 \brief command line module for RCP development
 *  \details
 *		This header contains the implementation of the debug menu interface.
 *
 *	\author Graham Riches
 */

/********************************** Includes *******************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "command_line.h"
#include "utilities.h"
#include "debug.h"
#include "usart.h"
#include "gpio.h"
#include "accelerometer.h"
#include "adc.h"

/********************************** Constants *******************************************/
#define COMMAND_BUFFER_SIZE 256

/********************************** Local Variables *******************************************/
static char msgIn[COMMAND_BUFFER_SIZE] = {0};
static uint16_t bytesReceived = 0;

/*********************************** Local Functions ********************************************/
int32_t  CLI_parseCommand( char *commandString, int *argc, char *argv[] );


/************************************ Command Line Definitions ********************************************/

/******************** Help Command ********************/
static const char help_name[] = "help";
static const char * const help_args[] = {"None", 0};
static const char help_desc[] = "List CLI commands";
static int help_func( int argc, char *argv[] )
{
   PARAMETER_NOT_USED( argc );
   PARAMETER_NOT_USED( argv );
   CLI_listCommands();
   return 0;
}

const CLI_command_t help =
{
   help_func,
   help_name,
   help_args,
   help_desc
};

/******************** LED Control Command ********************/
static const char led_name[] = "led";
static const char * const led_args[] = {"[COLOR]","[ON/OFF]", 0};
static const char led_desc[] = "Enable/Disable LEDs";
static int led_func( int argc, char *argv[] )
{
   if ( argc < 2 )
   {
       DEBUG_print( "Invalid number of arguments\n" );
   }

   GPIO_LED_t led = strtol( argv[1], NULL, 10 );
   GPIO_LED_state_t state = strtol( argv[2], NULL, 10 );
   GPIO_setLED( led, state );

   return 0;
}

const CLI_command_t led =
{
   led_func,
   led_name,
   led_args,
   led_desc
};

/******************** Accelerometer Command ********************/
static const char accel_name[] = "accel";
static const char * const accel_args[] = {"[MODE]", 0};
static const char accel_desc[] = "Accelerometer options";
static int accel_func( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        DEBUG_print( "Invalid number of arguments\n" );
    }

    if ( strncasecmp(argv[1], "test", CLI_MAX_ARG_LEN) == 0 )
    {
        ACCEL_test();
    }
    if ( strncasecmp(argv[1], "control", CLI_MAX_ARG_LEN) == 0 )
    {
        ACCEL_setControlMode( (ACCEL_controlMode_t)strtol(argv[2], NULL, 10) );
    }
    return 0;
}

const CLI_command_t accel =
{
    accel_func,
    accel_name,
    accel_args,
    accel_desc
};

/******************** ADC Command ********************/
static const char adc_name[] = "adc";
static const char * const adc_args[] = {"[measure]", 0};
static const char adc_desc[] = "ADC options";
static int adc_func( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        DEBUG_print( "Invalid number of arguments\n" );
    }

    if ( strncasecmp(argv[1], "measure", CLI_MAX_ARG_LEN) == 0 )
    {
        uint32_t value = ADC_getValue( ADC_AUDIO_INPUT );
        /* ADC resolution is set to 12B, so convert to voltage */
        float voltage = (float)((float)value/(float)4096.0 * (float)2.93588);
        DEBUG_print( "ADC Voltage: %f \n", voltage );
    }

    return 0;
}

const CLI_command_t adc =
{
    adc_func,
    adc_name,
    adc_args,
    adc_desc
};


/************************************ Command Line List ********************************************/
/**
 * \brief contains the entire list of command line options used to initialize the CLI
 */
static const CLI_command_t *CLI_menuCommands[CLI_MAX_COMMANDS] =
{
   &help,
   &led,
   &accel,
   &adc,
   // add new CLI commands here
};



/************************************ Public Functions ********************************************/

/**
 * \brief initialize the CLI module with a list of commands
 */
bool CLI_init( const CLI_command_t **commandList )
{
    if ( commandList == NULL )
    {
        return false;
    }

    int i = 0;

    while ( commandList[i] != NULL )
    {
        if ( i > CLI_MAX_COMMANDS )
        {
          return false;
        }

        CLI_menuCommands[i] = commandList[i];
        i++;
    }
    return true;
}


/**
 * \brief function to call when a UART RX event occurs
 */
void CLI_mainEventFunc( void )
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


/**
 * \brief call a command from the command list based on arguments
 * \param commandList list of CLI_command_t arguments
 * \param argc number of arguments
 * \param argv  arguments. First one should match the name field in CLI_command_t type
 * \retval returns the output from the function pointer callback
 */
int32_t CLI_executeCommand( char *commandString )
{
    int retval;
    int argc;
    char *argv[CLI_MAX_ARGS];

    retval = CLI_parseCommand( commandString, &argc, argv );
    if ( retval != 0 )
    {
       return retval;
    }


    int commandCount = 0;

    const CLI_command_t *currCommand = CLI_menuCommands[0];
    while ( currCommand != NULL )
    {
    if ( commandCount > CLI_MAX_COMMANDS )
    {
        return -1;
    }

    if ( strncasecmp( currCommand->name, argv[0], CLI_MAX_ARG_LEN ) == 0 )
    {
        /* call the function pointer */
        retval = currCommand->callback(argc, argv);
        break;
    }
    commandCount++;
    currCommand = CLI_menuCommands[commandCount];
    }
    return retval;
}


/**
 * \brief print out the command line options
 */
void CLI_listCommands( void )
{
    if ( CLI_menuCommands == NULL )
    {
        return;
    }
    int commandNum = 0;
    const CLI_command_t *currCommand = CLI_menuCommands[0];

    while ( currCommand != NULL )
    {
        if ( commandNum > CLI_MAX_COMMANDS )
        {
            return;
        }

        DEBUG_print("COMMAND: %s\r\n", currCommand->name);
        DEBUG_print("   Description: %s\r\n", currCommand->description);
        DEBUG_print("   ARGS: \r\n");

        int i = 0;
        while ( currCommand->usage[i] != NULL )
        {
            if ( i > CLI_MAX_ARGS )
            {
                break;
            }
            DEBUG_print("      %s\r\n", currCommand->usage[i] );
            i++;
        }
        DEBUG_print("\r\n");
        commandNum++;
        currCommand = CLI_menuCommands[commandNum];
    }
    return;
}

/*********************************** Local Function Definitions ********************************************/
/**
 * \brief parse a command line argument
 * \param commandString --> the command line stringvirt
 * \param argc --> output of the number of parsed arguments
 * \param argv --> array of argument strings
 */
int32_t CLI_parseCommand( char *commandString, int *argc, char *argv[] )
{
   *argc = UTILITIES_strSplit ( commandString, ' ', CLI_MAX_ARGS, argv );
   if ( *argc == 0 )
   {
      return -1;
   }
   else
   {
      return 0;
   }
}

