/*! \file utilities.c
*
*  \brief contains useful utilities that are shared by different submodules throughout the project.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
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
 * \brief add data to a ring buffer
 * \param buffer pointer to the buffer handler
 * \param data pointer to the data buffer to add
 * \param size how much data to add
 * \retval UTILITIES_ringBufferStatus_t enum type
 */
UTILITIES_ringBufferStatus_t UTILITIES_ringBufferAdd( UTILITIES_ringBuffer_t *buffer, uint8_t *data, uint16_t size )
{
    UTILITIES_ringBufferStatus_t status = UTILITIES_RINGBUFFER_OK;

    if ( (buffer == NULL) || (data == NULL) || (size == 0) )
    {
        return UTILITIES_RINGBUFFER_NULL;
    }

    /* check if there is space in the buffer */
    if ( UTILITIES_ringBufferGetFreeSpace( buffer ) < size )
    {
        return UTILITIES_RINGBUFFER_FULL;
    }

    /* add the data */
    if ( (buffer->head + size) <= buffer->size )
    {
        /* simply add to the head if there is room */
        memcpy( &buffer->data[buffer->head], data, size );
        buffer->head += size;
    }
    else
    {
        /* handle wrap around */
        memcpy( &buffer->data[buffer->head], data, buffer->size - buffer->head );
        memcpy( &buffer->data[0], &data[buffer->size - buffer->head], size - (buffer->size - buffer->head) );
        buffer->head = size - (buffer->size - buffer->head);
    }

    return status;
}


/**
 * \brief get data from a ring buffer
 * \param buffer pointer to the ring buffer handler
 * \param data pointer to the receivers data buffer
 * \param size max amount of data to receive
 * \retval number of bytes received
 */
uint16_t UTILITIES_ringBufferGet( UTILITIES_ringBuffer_t *buffer, uint8_t *data, uint16_t size )
{
    uint16_t retval = 0;
    uint16_t dataAvailable = 0;

    if ( (buffer == NULL) || (data == NULL) || (size == 0) )
    {
        return 0;
    }

    /* check that we aren't requesting more data than the buffer has available */
    dataAvailable = UTILITIES_ringBufferGetUsedSpace( buffer );

    if ( size > dataAvailable )
    {
        size = dataAvailable;
    }

    /* copy the data into the receivers buffer */
    if ( (buffer->head > buffer->tail) || (size <= (buffer->size - buffer->tail)) )
    {
        memcpy( data, &buffer->data[buffer->tail], size );
        buffer->tail += size;
    }
    else
    {
        /* handle wrap around */
        memcpy( data, &buffer->data[buffer->tail], buffer->size - buffer->tail );
        memcpy( &data[buffer->size - buffer->tail], &buffer->data[0], size - (buffer->size - buffer->tail));
        buffer->tail = size - (buffer->size - buffer->tail);
    }
    return size;
}


/**
 * \brief get how much free space there is in a buffer
 * \param buffer pointer to the ring buffer handler
 * \retval number of bytes available
 */
uint16_t UTILITIES_ringBufferGetFreeSpace( UTILITIES_ringBuffer_t *buffer )
{
    uint16_t freeSize = 0;
    if ( buffer == NULL )
    {
        return freeSize;
    }

    freeSize = buffer->size - UTILITIES_ringBufferGetUsedSpace( buffer ) -1 ;
    return freeSize;
}


/**
 * \brief get how much space is used in a buffer
 * \param buffer the pointer to the ring buffer
 * \retval number of bytes used
 */
uint16_t UTILITIES_ringBufferGetUsedSpace( UTILITIES_ringBuffer_t *buffer )
{
    uint16_t usedSize = 0;
    if ( buffer == NULL )
    {
        return 0;
    }

    if ( buffer->head >= buffer->tail )
    {
        usedSize = buffer->head - buffer->tail;
    }
    else
    {
        /* handle wrap around */
        usedSize = buffer->size - buffer->tail  + buffer->head;
    }
    return usedSize;
}


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
