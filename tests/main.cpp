/*! \file main.cpp
*
*  \brief main test runner for all unit tests
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "gtest/gtest.h"

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief main test runner function
 * 
 * \param argc number of command line options
 * \param argv pointer to each command line argument
 * \retval int 
 */
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
