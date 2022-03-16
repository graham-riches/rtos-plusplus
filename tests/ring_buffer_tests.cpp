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
#include "ring_buffer.h"
#include "gtest/gtest.h"
#include <array>
#include <vector>

/*********************************** Consts ********************************************/

/*********************************** Test Fixtures ********************************************/
/**
 * \brief test fixture for ring buffer tests
 */
class RingBufferTests : public ::testing::Test {
  public:
    void SetUp() override { }

    void TearDown() override { }

    ring_buffer<int, 5> buffer;
};

TEST_F(RingBufferTests, test_initial_construction) {
    ASSERT_FALSE(buffer.full());
    ASSERT_TRUE(buffer.empty());
}

TEST_F(RingBufferTests, test_push_increments_size) {
    std::array<int, 5> values = {1, 2, 3, 4, 5};
    for ( auto val : values ) {
        buffer.push_back(val);
        ASSERT_EQ(buffer.size(), val);
    }
}

TEST_F(RingBufferTests, test_buffer_wrap_does_not_change_size) {
    std::array<int, 5> values = {1, 2, 3, 4, 5};
    for ( auto val : values ) {
        buffer.push_back(val);
        ASSERT_EQ(buffer.size(), val);
    }
    buffer.push_back(6);
    ASSERT_EQ(buffer.size(), 5);
}

TEST_F(RingBufferTests, test_begin_iterator_gets_first_element) {
    buffer.push_back(1);
    auto it = buffer.begin();
    ASSERT_EQ(*it, 1);
}

TEST_F(RingBufferTests, test_begin_iterator_gets_first_element_after_wrapping) {
    ring_buffer<int, 2> buff;
    buff.push_back(1);
    buff.push_back(2);
    buff.push_back(3);  // Has now wrapped
    auto it = buff.begin();
    ASSERT_EQ(*it, 2);
}

TEST_F(RingBufferTests, test_buffer_in_range_based_for) {
    std::array<int, 5> values = {1, 2, 3, 4, 5};
    for ( auto val : values ) {
        buffer.push_back(val);
    }
    unsigned count = 0;
    for ( auto val : buffer ) {
        ASSERT_EQ(val, values[count++]);        
    }
    ASSERT_EQ(5, values[count]);
}

TEST_F(RingBufferTests, test_creating_const_iterators) {
    buffer.push_back(1);
    auto it = buffer.cbegin();
    ASSERT_EQ(*it, 1);
    it = buffer.cend();
    ASSERT_EQ(*it, 1);
}

TEST_F(RingBufferTests, test_reverse_iterators) {
    std::array<int, 5> values = {1, 2, 3, 4, 5};
    for ( auto val : values ) {
        buffer.push_back(val);
    }
    auto rev_array = values.rbegin();
    auto temp = *rev_array;
    for (auto it = buffer.rbegin(); it != buffer.rend(); it++) {
        temp = *rev_array;
        ASSERT_EQ(*it, *rev_array++);        
    }
    ASSERT_EQ(temp, 1);
}

TEST_F(RingBufferTests, test_const_reverse_iterators) {
    std::array<int, 5> values = {1, 2, 3, 4, 5};
    for ( auto val : values ) {
        buffer.push_back(val);
    }
    auto rev_array = values.rbegin();
    auto temp = *rev_array;
    for (auto it = buffer.crbegin(); it != buffer.crend(); it++) {
        temp = *rev_array;
        ASSERT_EQ(*it, *rev_array++);        
    }
    ASSERT_EQ(temp, 1);
}