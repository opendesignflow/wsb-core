/**
 * VDOMNodeList.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

#include "VDOMElement.h"
#include "VDOMNodeList.h"

namespace VDOM {

VDOMNodeList::VDOMNodeList() {
    // TODO Auto-generated constructor stub

}

VDOMNodeList::~VDOMNodeList() {
    // TODO Auto-generated destructor stub
}


size_t VDOMNodeList::getLength() {
    return this->size();
}


template<> VDOMNode * VDOMNodeList::getItem<VDOMNode>(size_t index) throw (out_of_range) {

        return *(this->begin()+index);
}

template<> VDOMElement  * VDOMNodeList::getItem<VDOMElement>(size_t index) throw (out_of_range) {

    return dynamic_cast<VDOMElement*>(*(this->begin()+index));
}


} /* namespace VDOM */
