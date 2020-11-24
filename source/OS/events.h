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
};

/**
 * \brief wait for all flags to be set, or any combination of flags
 */
enum class EventGetOptions : unsigned
{
   get_all = 0,
   get_any,
};

/**
 * \brief structure to handle threads pending on an event flags group
 */
struct EventFlagsControlBlock
{
   Thread *suspended_thread;           //!< pointer to the suspended thread
   uint32_t pending_flags;             //!< which flags to wait for
   EventWaitOptions wait_option;       //!< thread suspension wait options
   EventGetOptions get_option;         //!< thread suspension get options
   uint32_t suspended_ticks_remaining; //!< number of time ticks left before a thread will un-suspend itself
   EventFlagsControlBlock *next;       //!< pointer to the next event flags control block
   EventFlagsControlBlock *previous;   //!< pointer to the previous control block
};

/**
 * \brief event flags class to manage thread synchronization
 */
class EventFlags
{
   private:
   /* private data */
   uint32_t flags;
   EventFlagsControlBlock thread_control[system_max_threads];
   EventFlagsControlBlock *suspended_threads;
   ThreadManager *thread_manager;

   /* private methods */
   void clear_pending_thread( EventFlagsControlBlock *control_block );
   void check_suspended_threads( void );
   bool check_flags( uint32_t flags, EventGetOptions get_options );

   public:
   EventFlags( ThreadManager *thread_manager );
   void set( uint32_t flags );
   void clear( uint32_t flags );
   void get( uint32_t flags, EventGetOptions get_options, EventWaitOptions wait_options );
   void get( uint32_t flags, EventGetOptions get_options, EventWaitOptions wait_options, uint32_t wait_time_ms );
};

};  // namespace OS

#endif /* __EVENTS_H__ */
