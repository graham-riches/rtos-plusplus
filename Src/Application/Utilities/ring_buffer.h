/*! \file ring_buffer.h
*
*  \brief ring_buffer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

/********************************** Includes *******************************************/
#include "common.h"
#include <memory>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief template class for a re-usable ring buffer
 * 
 * \tparam T parameter type
 */
template <typename T>
class RingBuffer
{
   private:
   std::unique_ptr<T[]> buffer;
   const size_t max_size;
   size_t head = 0;
   size_t tail = 0;
   bool full = false;
   

   public:
      /* constructor */
      explicit RingBuffer( size_t size )
         : buffer( std::make_unique<T[]>( size ) )
         , max_size( size )
      { }

      void put( T data );
      T get( void );
      bool is_empty( void );
      bool is_full( void );
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/

#endif /* __RING_BUFFER_H__ */
