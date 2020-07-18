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
#include "command_line.h"
#include "utilities.h"
#include "debug.h"


/*********************************** Local Functions ********************************************/
int32_t  CLI_parseCommand( char *commandString, int *argc, char *argv[] );


/************************************ Command Line Definitions ********************************************/

/******************** Help Command ****************************************/
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

/************************************ Command Line List ********************************************/
/**
 * \brief contains the entire list of command line options used to initialize the CLI
 */
static const CLI_command_t *CLI_menuCommands[CLI_MAX_COMMANDS] =
{
   &help,
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
            printf("      %s\r\n", currCommand->usage[i] );
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

