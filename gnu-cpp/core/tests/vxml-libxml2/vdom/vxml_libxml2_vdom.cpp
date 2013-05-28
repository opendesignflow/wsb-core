/**
 * simple_dom.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Gtest
#include <gtest/gtest.h>

#include <vxml/vdom/VDOMRegistry.h>
#include <vxml-libxml2/vdom/DOMImplementationFactory.h>

/**
 * Test registration in registry
 */
TEST(libxml2_vdom, registry) {


    //-- Add to registry
    VDOM::VDOMRegistry::getInstance().registerImplementation(new VDOM::LIBXML2::DOMImplementationFactory());

    //-- Create an implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");
    ASSERT_TRUE(libxml2impl!=NULL);

    ASSERT_NO_THROW(delete libxml2impl);

}
