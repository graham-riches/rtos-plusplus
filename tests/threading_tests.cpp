/*! \file events_tests.cpp
*
*  \brief Unit tests for the event flags group.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "thread_impl.h"
#include "common.h"
#include <cstdint>
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/
constexpr uint16_t thread_stack_size = 512;
constexpr uint8_t thread_count = 2;

/************************************ Test Fixtures ********************************************/
/**
 * \brief test fixture class for unit testing some of the threading primitives 
*/
class ThreadingTests : public ::testing::Test {
  protected:
    static void thread_task(void *arguments){ PARAMETER_NOT_USED(arguments); };    

    void SetUp(void) override {        
        thread_stack = std::make_unique<uint32_t[]>(thread_stack_size);
        thread = create_thread(reinterpret_cast<os::thread::task_pointer>(&thread_task), nullptr, 1, thread_stack.get(), thread_stack_size);
    }

  public:
    std::unique_ptr<os::thread> thread;
    std::unique_ptr<uint32_t[]> thread_stack;

    std::unique_ptr<os::thread> create_thread(os::thread::task_pointer task_ptr, void *args, uint8_t thread_id, uint32_t *stack_ptr, uint32_t stack_size) {
        return std::make_unique<os::thread>(task_ptr, args, thread_id, stack_ptr, stack_size);
    }
};

using ThreadingDeathsTests = ThreadingTests;

/************************************ Tests ********************************************/
TEST_F(ThreadingTests, test_thread_is_default_pending) {
    ASSERT_EQ(os::thread::status::pending, thread->get_status());
}

TEST_F(ThreadingTests, test_set_thread_status) {
    thread->set_status(os::thread::status::active);
    ASSERT_EQ(os::thread::status::active, thread->get_status());
}

TEST_F(ThreadingTests, test_initial_stack_pointer_dne_stack_top){
    uint32_t* initial_stack_ptr = thread->get_stack_ptr();
    ASSERT_TRUE(initial_stack_ptr != thread_stack.get());
}

TEST_F(ThreadingTests, test_initial_stack_context_is_valid){
    uint32_t *stack_context_ptr = thread->get_stack_ptr();
    os::thread::register_context* context = reinterpret_cast<os::thread::register_context*>(stack_context_ptr);
    ASSERT_EQ(4, context->r4);
    ASSERT_EQ(5, context->r5);
    ASSERT_EQ(0x01000000, context->psr);
    ASSERT_EQ( static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(&thread_task)), context->pc);
}

TEST_F(ThreadingDeathsTests, creating_thread_with_null_task_ptr_fails){    
    ASSERT_DEATH({create_thread(nullptr, nullptr, 1, thread_stack.get(), thread_stack_size);}, "");
}

TEST_F(ThreadingDeathsTests, creating_thread_with_null_stack_fails){
    ASSERT_DEATH({create_thread(reinterpret_cast<os::thread::task_pointer>(&thread_task), nullptr, 1, nullptr, thread_stack_size);}, "");
}

TEST_F(ThreadingDeathsTests, creating_thread_with_empty_stack_size_fails){
    ASSERT_DEATH({create_thread(reinterpret_cast<os::thread::task_pointer>(&thread_task), nullptr, 1, thread_stack.get(), 0);}, "");
}