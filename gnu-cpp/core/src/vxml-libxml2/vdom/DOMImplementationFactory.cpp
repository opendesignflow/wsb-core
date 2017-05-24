/**
 * DOMImplementationFactory.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------



#include "DOMImplementation.h"
#include "DOMImplementationFactory.h"

namespace VDOM {
namespace LIBXML2 {

DOMImplementationFactory::DOMImplementationFactory() {

    xmlInitParser();
    LIBXML_TEST_VERSION

}

DOMImplementationFactory::~DOMImplementationFactory() {
    // TODO Auto-generated destructor stub
}

VDOMImplementation * DOMImplementationFactory::createInstance() {

    return new DOMImplementation();
}



const char * DOMImplementationFactory::getName() {
    return "libxml2.dom";
}

} /* namespace LIBXML2 */
} /* namespace VDOM */
