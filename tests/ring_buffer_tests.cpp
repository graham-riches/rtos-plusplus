/**
 * \file ring_buffer_tests.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief unit tests for the ring buffer class
 * \version 0.1
 * \date 2021-05-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "ring_buffer.h"
#include <vector>


/*********************************** Consts ********************************************/

/*********************************** Test Fixtures ********************************************/
/**
 * \brief test fixture for ring buffer tests
 */
class RingBufferTests : public ::testing::Test {
    public:
    void SetUp() override {}

    void TearDown() override {}
    
    RingBuffer<int> buffer{5};
};


TEST_F(RingBufferTests, test_initial_construction) {
    ASSERT_FALSE(buffer.is_full());
    ASSERT_TRUE(buffer.is_empty());
}

TEST_F(RingBufferTests, test_filling_buffer_returns_full) {
    std::vector<int> values = {0, 1, 2, 3, 4};
    for (const auto& val : values) {
        ASSERT_TRUE(buffer.push(val));
    }

    ASSERT_TRUE(buffer.is_full());
    ASSERT_FALSE(buffer.is_empty());

    for (const auto& val : values) {
        ASSERT_EQ(val, buffer.pop().value());
    }
}

TEST_F(RingBufferTests, test_trying_push_to_full_buffer_fails) {
    std::vector<int> values = {0, 1, 2, 3, 4};
    for (const auto& val : values) {
        ASSERT_TRUE(buffer.push(val));
    }
    ASSERT_FALSE(buffer.push(5));
}

TEST_F(RingBufferTests, test_put_then_pop_pointer_wrap_around_works) {
    std::vector<int> values = {0, 1, 2, 3, 4};
    for (const auto& val : values) {
        buffer.push(val);
    }
    buffer.pop();

    buffer.push(5);
    ASSERT_TRUE(buffer.is_full());
}

TEST_F(RingBufferTests, test_flush_buffer) {
    std::vector<int> values = {0, 1, 2, 3, 4};
    for (const auto& val : values) {
        buffer.push(val);
    }
    buffer.flush();
    ASSERT_TRUE(buffer.is_empty());
    ASSERT_FALSE(buffer.is_full());
}