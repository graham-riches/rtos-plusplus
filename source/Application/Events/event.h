/*! \file event.h
*
*  \brief event module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __EVENT_H__
#define __EVENT_H__

/********************************** Includes *******************************************/
#include "common.h"
#include <functional>

/*********************************** Consts ********************************************/
#define MAX_EVENT_GROUP_SIZE 32  //!< max number of events in a group is 32 (using 32-bit variable for flags)

/************************************ Types ********************************************/

namespace OS
{

class EventFlags
{
   private:   

   public:
   EventFlags( ) { }

   void register_event( std::function<void( void )> callback );
   void set( );
   void get( );
};
};  // namespace OS

#endif /* __EVENT_H__ */
