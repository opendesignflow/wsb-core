/**
 * VDOMImplementation.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <sstream>
#include <stdexcept>
using namespace std;

#include "VDOMImplementation.h"

namespace VDOM {


VDOMImplementation::VDOMImplementation() {

}

VDOMImplementation::~VDOMImplementation() {

}


/**
 * Calls #parseDocument(istream) with string converted into a stream
 * @param xml
 * @return
 */
VDOMDocument * VDOMImplementation::parseDocument(istream & in) {

    throw runtime_error("not implemented");

}

}
