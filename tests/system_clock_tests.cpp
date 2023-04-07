// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "system_clock.hpp"
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/


/************************************ Test Fixtures ********************************************/
/**
 * \brief test fixture class for unit testing the system clock
*/
class SystemClockTests : public ::testing::Test {
  protected:
    static void thread_task(void *arguments){ (void)(arguments); };    

    void SetUp(void) override {        
        
    }

  public:
      os::system_clock clock;    
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
    clock.update(1);
    ASSERT_EQ(1, clock.get_ticks());
}
