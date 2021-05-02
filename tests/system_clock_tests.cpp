/*! \file events_tests.cpp
*
*  \brief Unit tests for the event flags group.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "system_clock.h"
#include "common.h"
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/


/************************************ Test Fixtures ********************************************/
/**
 * \brief test fixture class for unit testing the system clock
*/
class SystemClockTests : public ::testing::Test {
  protected:
    static void thread_task(void *arguments){ PARAMETER_NOT_USED(arguments); };    

    void SetUp(void) override {        
        
    }

  public:
      os::SystemClock clock;    
};



/************************************ Tests ********************************************/
TEST_F(SystemClockTests, clock_initializes_ticks_to_zero) {
    ASSERT_EQ(0, clock.get_ticks());
}

TEST_F(SystemClockTests, test_update_clock_ticks_not_running_fails){
    clock.update(1);
    ASSERT_EQ(0, clock.get_ticks());
}

TEST_F(SystemClockTests, test_update_clock_running_passes) {
    clock.start();
    clock.update(1);
    ASSERT_EQ(1, clock.get_ticks());
}
