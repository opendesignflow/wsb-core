/**
 * VXPathResult.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------
#include "VXPathResult.h"

namespace VDOM {

VXPathResult::VXPathResult() {
    // TODO Auto-generated constructor stub

}

VXPathResult::~VXPathResult() {
    // TODO Auto-generated destructor stub
}

VDOMNodeList & VXPathResult::getNodeList() {
    return this->nodesList;
}

} /* namespace VDOM */
