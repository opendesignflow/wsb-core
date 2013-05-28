/**
 * VDOMElement.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------
#include "VDOMElement.h"

namespace VDOM {

VDOMElement::VDOMElement() {
    // TODO Auto-generated constructor stub

}

VDOMElement::~VDOMElement() {
    // TODO Auto-generated destructor stub
}

VDOMNode::NodeType VDOMElement::getNodeType() {
    return VDOMNode::ELEMENT_NODE;
}

void VDOMElement::deleteAttribute(string name) {
    this->setAttribute(name,"");
}

void VDOMElement::setTextContent(const char * content) {

    string ct(content);
    this->setTextContent(ct);

}

} /* namespace VDOM */
