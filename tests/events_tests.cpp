/*! \file events_tests.cpp
*
*  \brief Unit tests for the event flags group.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "gtest\gtest.h"
#include "events.h"
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/
constexpr uint8_t thread_stack_size = 128;
constexpr uint32_t test_event_one = 0x01;
constexpr uint32_t test_event_two = 0x02;
constexpr uint32_t test_event_three = 0x03;

/************************************ Test Fixtures ********************************************/
/**
 * \brief test class for the event flags class. This sets up some fake threads and a thread manager
 *        to test suspending threads on event flags.
*/
class EventFlagsTest : public ::testing::Test {
  protected:
    std::unique_ptr<os::EventFlags> event_flags;
    std::unique_ptr<os::thread> thread_one;
    std::unique_ptr<os::thread> thread_two;
    std::unique_ptr<os::threadManager> thread_manager;
    std::unique_ptr<uint32_t[]> thread_one_stack;
    std::unique_ptr<uint32_t[]> thread_two_stack;

    static void thread_one_function(void){};
    static void thread_two_function(void){};

    void SetUp(void) override {}

    void TearDown(void) override {}
};

/************************************ Tests ********************************************/
TEST_F(EventFlagsTest, test_event_flags_get_suspends_thread) {
}
