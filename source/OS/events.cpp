/*! \file events.cpp
*
*  \brief OS event flags API.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "events.h"

namespace OS
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new Event Flags:: Event Flags object
 * \param thread_manager system thread manager reference
 */
EventFlags::EventFlags( ThreadManager *thread_manager )
{
   this->thread_manager = thread_manager;
   this->flags = 0;
   for ( uint8_t i = 0; i < system_max_threads; i++ )
   {
      this->thread_control[i].next = nullptr;
      this->thread_control[i].previous = nullptr;
   }
}

/**
 * \brief remove a thread that was pending on the current event flags group by removing
 *        it from the queue
 * 
 * \param control_block the control block to pop
 */
void EventFlags::clear_pending_thread( EventFlagsControlBlock *control_block )
{

}


/**
 * \brief check a set of event flags with a certain wait option
 * 
 * \param flags the flags to check
 * \param get_options get options (get all, get any)
 * \retval bool if a match is found
 */
bool EventFlags::check_flags( uint32_t flags, EventGetOptions get_options )
{
   bool check_result = false;
   switch ( get_options )
   {
      case EventGetOptions::get_all:
         check_result = ( ( this->flags & flags ) == flags );
         break;
      
      case EventGetOptions::get_any:
         check_result = ( this->flags & flags );
         break;
      
      default:
         break;
   }
   return check_result;
}

/**
 * \brief check any suspended threads against the current flags
 */
void EventFlags::check_suspended_threads( void )
{
   for ( uint8_t i = 0; i < this->suspended_threads; i++ )
   {
      EventFlagsControlBlock *current_block = &this->thread_control[i];

      if ( this->check_flags( current_block->pending_flags, current_block->get_option ) )
      {
         this->clear_pending_thread( current_block );
      }
   }
}

/**
 * \brief set any event flags and check if this will release any thread from the suspended state
 * 
 * \param flags the flags to set
 */
void EventFlags::set( uint32_t flags )
{
   this->flags |= flags;
   this->check_suspended_threads(); 
}
   
/**
 * \brief clear any event flags
 * 
 * \param flags flags to clear
 */
void EventFlags::clear( uint32_t flags )
{
   this->flags &= ~(flags);
}

/**
 * \brief suspend a calling thread on an eventflags object
 * 
 * \param flags flags to suspend on
 * \param get_options type of suspension (wait for any flag, or wait for all)
 * \param wait_options wait option
 */
void EventFlags::get( uint32_t flags, EventGetOptions get_options, EventWaitOptions wait_options )
{
   /* check if the flag criteria is already met */
   if ( !this->check_flags( flags, get_options) )
   {
      /* suspend the calling thread if the condition is not met */
      this->thread_manager->activeTask->thread->set_status( ThreadStatus::suspended );
   }
}


};  // namespace OS
