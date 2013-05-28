/**
 * XMLNode.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- VDOM
#include <vxml/vdom/VDOMDocument.h>

#include "XMLNode.h"

namespace OSI {
namespace WSB {

XMLNode::XMLNode() {

    // Create DOMDOcument
    //-------------------
    this->manipulator = new DOMManipulator();

    // Init base Element
    //----------------------
    this->baseElement = NULL;
    //this->initBaseElement();

    // Add to document
    //---------------
//    if (this->manipulator->getDocument()->getDocumentElement()==NULL)
//        this->manipulator->getDocument()->appendChild(this->baseElement);

}

XMLNode::XMLNode(VDOM::VDOMElement * baseElement) {


    // Create Manipulator from parentElement's DOMDOcument
    //-----------
    this->manipulator = new DOMManipulator(baseElement->getOwnerDocument());

    // Init base Element
    //----------------------
    this->baseElement = baseElement;
//    this->initBaseElement();


}

XMLNode::~XMLNode() {
    // TODO Auto-generated destructor stub
}


DOMManipulator & XMLNode::getDOMManipulator() {

    return *(this->manipulator);

}


string XMLNode::toXMLString() {


    // The manipulator has an integrated serialisation method
    return this->getDOMManipulator().toString();


}

} /* namespace WSB */
} /* namespace OSI */
