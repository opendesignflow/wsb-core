/**
 * BaseTestEngine.hpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef BASETESTENGINE_HPP_
#define BASETESTENGINE_HPP_

// Includes
//-----------------

//-- Wsb
#include <wsb-core/engine/WSEngine.h>

//-- Gtest
#include <gtest/gtest.h>

using namespace OSI::WSB;

/**
 * This is a WSEngine subclass to provide utility methods when used in the context of a GTest
 */
class BaseTestEngine: public WSEngine, public ::testing::Test {

    public:
        BaseTestEngine() {

        }

        virtual ~BaseTestEngine() {

        }

    protected:

        virtual void SetUp() {

        }

        virtual void TearDown() {

        }


};

#endif /* BASETESTENGINE_HPP_ */
