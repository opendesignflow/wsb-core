/**
 * VDOMDocument.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------
#include "VDOMDocument.h"

namespace VDOM {

VDOMDocument::VDOMDocument() {
    // TODO Auto-generated constructor stub

}

VDOMDocument::~VDOMDocument() {
    // TODO Auto-generated destructor stub
}

VDOMNode::NodeType VDOMDocument::getNodeType() {
    return VDOMNode::DOCUMENT_NODE;
}

} /* namespace VDOM */
