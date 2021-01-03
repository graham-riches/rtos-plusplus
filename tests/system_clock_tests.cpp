/*! \file events_tests.cpp
*
*  \brief Unit tests for the event flags group.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "gtest\gtest.h"
#include "system_clock.h"
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/


/************************************ Test Fixtures ********************************************/
/**
 * \brief test fixture class for unit testing some of the threading primitives 
*/
class SystemClockTests : public ::testing::Test {
  protected:
    static void thread_task(void *arguments){};    

    void SetUp(void) override {        
        
    }

  public:
      OS::SystemClock clock;    
};



/************************************ Tests ********************************************/
TEST_F(SystemClockTests, clock_initializes_ticks_to_zero) {
    ASSERT_EQ(0, clock.get_ticks());
}

TEST_F(SystemClockTests, test_update_clock_ticks){
    clock.update(1);
    ASSERT_EQ(1, clock.get_ticks());
}
