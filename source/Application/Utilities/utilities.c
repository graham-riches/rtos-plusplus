/*! \file utilities.c
*
*  \brief contains useful utilities that are shared by different submodules throughout the project.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "utilities.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     UTILITIES_strSplit
* \brief    Uses delimiter to split inputString non destructively (replaces delimiters with \0)
*
* \param    inputString: Pointer to the input string with delimiters. Must end in '\0'.
* \param    delimiter: The character used to split inputString into sub strings
* \param    maxStrings: The maximum array size of argv[]
* \param    argv: An array of char pointers, pointing to various sub strings in inputString
* \retval   int32_t: Number of arguments in argv[]. Returns 0 if inputString is empty or there's an error
*/
int32_t UTILITIES_strSplit ( char *inputString, char delimiter, int maxStrings, char *argv[] )
{
    char *currentCharPtr;
    int argCounter;
    int characterCounter;

    /* Check for NULL pointers */
    if ( ( inputString == NULL ) || ( argv == NULL ) || ( maxStrings == 0 ) )
    {
        return 0;
    }

    /* Special case, catch if *inputString's first char is a '\0' */
    if ( *inputString == '\0' )
    {
        return 0;
    }

    argCounter = 0;
    characterCounter = 0;
    currentCharPtr = inputString; // Start at the first character of the input string

    /* argv[0] starts at the beginning of the string */
    argv[ argCounter ] = currentCharPtr;

    /* Loop through every character in the string until we hit the end */
    while ( ( *currentCharPtr != '\0' ) && ( characterCounter++ < UTILITIES_MAX_STRSPLIT_LENGTH ) )
    {
        if ( *currentCharPtr == delimiter ) // We hit a delimiter character
        {
            *currentCharPtr = '\0'; // Swap the delimiter with a null terminator
            ++currentCharPtr;       // Increment the character pointer
            ++argCounter;

            // Update argv[] if we have enough room
            if ( argCounter < maxStrings )
            {
                argv [ argCounter ] = currentCharPtr;
            }
            else
            {
                return argCounter; // Not enough room, just return the number of strings we split
            }
        }
        else // No delimiter, keep scanning
        {
            ++currentCharPtr;
        }
    }
    return ++argCounter; // Increment argCounter to return N, since we started at count 0
}
