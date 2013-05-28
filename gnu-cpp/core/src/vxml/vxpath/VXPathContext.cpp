/**
 * VXPathContext.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Common
#include <wsb-core/common/SGC.h>

//-- VDOM
#include "VDOMElement.h"
#include "VXPathContext.h"

namespace VDOM {

VXPathContext::VXPathContext() {
    // TODO Auto-generated constructor stub

}

VXPathContext::~VXPathContext() {
    // TODO Auto-generated destructor stub
}

void VXPathContext::importNSContext(VXPathNSContext * nsContext) {

    for (map<string,string>::iterator it = nsContext->begin(); it != nsContext->end() ; it++) {

        this->addNamespaceBinding((*it).first,(*it).second);

    }

}

VDOMElement * VXPathContext::evaluateAsElement(string xpath) {

    //-- Call normal evaluate
    OSI::WSB::LGC<VXPathResult> result(this->evaluate(xpath));

    //-- Try to get a result
    VDOMElement * element = NULL;
    if (result.getReference()!=NULL && result->getNodeList().size()>0 && result->getNodeList().at(0)->getNodeType() == VDOMNode::ELEMENT_NODE) {
        element = dynamic_cast<VDOMElement*>(result->getNodeList().at(0));
    }

    return element;

}

bool VXPathContext::evaluateAsBoolean(string xpath) {

    //-- Call normal evaluate
    OSI::WSB::LGC<VXPathResult> result(this->evaluate(xpath));

    //-- true if nodeset is not empty, false otherwise
    bool boolResult = result.getReference() != NULL && result->getNodeList().getLength()>0 ? true : false;



    return boolResult;

}

} /* namespace VDOM */
