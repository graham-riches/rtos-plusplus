/*! \file events.h
*
*  \brief events module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __EVENTS_H__
#define __EVENTS_H__

/********************************** Includes *******************************************/
#include "common.h"
#include "threading.h"

namespace OS
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief options for suspending a thread on an event flag
 */
enum class EventWaitOptions : unsigned
{
   wait_forever = 0,
   wait_timed,
   wait_none,
};


/**
 * \brief event flags to manage thread synchronization
 */
class EventFlags
{
   private:
   uint32_t flags;

   public:
   EventFlags( void )
   {
      this->flags = 0;
   }
   void set( uint32_t flags );
   void get( uint32_t flags, );
};

};  // namespace OS

#endif /* __EVENTS_H__ */
