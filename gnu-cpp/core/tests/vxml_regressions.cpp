/**
 * wsb_core_regressions.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */
// Includes
//-----------------
//-- Version
//#include <Version.h>
#include <gtest/gtest.h>

#include <vxml/vdom/VDOMRegistry.h>
#include <vxml-libxml2/vdom/DOMImplementationFactory.h>

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);



    //-- Init DOM Implementation
    VDOM::VDOMRegistry::getInstance().registerImplementation(new VDOM::LIBXML2::DOMImplementationFactory());

    return RUN_ALL_TESTS();
}
