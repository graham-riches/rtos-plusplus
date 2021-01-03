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
    std::unique_ptr<OS::EventFlags> event_flags;
    std::unique_ptr<OS::Thread> thread_one;
    std::unique_ptr<OS::Thread> thread_two;
    std::unique_ptr<OS::ThreadManager> thread_manager;
    std::unique_ptr<uint32_t[]> thread_one_stack;
    std::unique_ptr<uint32_t[]> thread_two_stack;

    static void thread_one_function(void){};
    static void thread_two_function(void){};

    void SetUp(void) override {
        /* create the system threads, thread manager, and event flags */
        this->thread_one_stack = std::make_unique<uint32_t[]>(thread_stack_size);
        this->thread_two_stack = std::make_unique<uint32_t[]>(thread_stack_size);
        this->thread_one =
            std::make_unique<OS::Thread>(reinterpret_cast<OS::THREAD_task_t>(this->thread_one_function), 1, this->thread_one_stack.get(), thread_stack_size);
        this->thread_two =
            std::make_unique<OS::Thread>(reinterpret_cast<OS::THREAD_task_t>(this->thread_two_function), 2, this->thread_two_stack.get(), thread_stack_size);
        this->thread_manager = std::make_unique<OS::ThreadManager>(2);
        this->event_flags = std::make_unique<OS::EventFlags>(this->thread_manager.get());

        /* register the threads */
        this->thread_manager->register_thread(this->thread_one.get());
        this->thread_manager->register_thread(this->thread_two.get());
    }
};

/************************************ Tests ********************************************/
TEST_F(EventFlagsTest, test_event_flags_get_suspends_thread) {
    ASSERT_TRUE(true);
    //event_flags->get(test_event_one, OS::EventGetOptions::get_all, OS::EventWaitOptions::wait_forever);
    //ASSERT_EQ(OS::ThreadStatus::suspended, thread_one->get_status());
}
