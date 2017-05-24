/**
 * DOMNode.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <typeinfo>
#include <cstring>
#include <sstream>
#include <iostream>
using namespace std;

//-- VDOM
#include <vxml/vdom/VDOMNodeList.h>

//-- LIBXML2 Implementation
#include "DOMDocument.h"
#include "DOMNode.h"
#include "DOMElement.h"

namespace VDOM {
namespace LIBXML2 {


DOMNode::DOMNode(xmlNodePtr node,DOMDocument * ownerDocument) {
    // TODO Auto-generated constructor stub
    this->nodePtr = node;
    this->document = ownerDocument;
}

DOMNode::~DOMNode() {

    /*if (this->nodePtr!=NULL)
        xmlFreeNode(this->nodePtr);*/
}

VDOMNode * DOMNode::cloneNode(bool recursive) {

    //-- Do copy of element or document
    if (this->nodePtr!=NULL) {

        xmlNodePtr newNode = xmlCopyNode(this->nodePtr,recursive);
        return DOMNode::toNode(newNode,this->document);

    } else {

        xmlDocPtr newDoc = xmlCopyDoc(this->document->getDocumentPointer(),recursive);
        return new DOMDocument(newDoc);
    }

}

VDOMNode * DOMNode::toNode(xmlNodePtr node,DOMDocument * document)  throw (invalid_argument){

    //-- Check
    if (node == NULL) {
        throw invalid_argument("Cannot call VDOM::LIBXML2::DOMNode::toNode on a NULL node pointer");
    }

    //-- Convert
    //-----------------
    VDOMNode * result = NULL;
    switch (node->type) {



        case XML_DOCUMENT_NODE:
        case XML_ELEMENT_NODE:

            //-- Create or get from pool
            result = document->getNodePool().get(node);
            if (result == NULL) {

                // Create
                result= new DOMElement(node,document);
                // Add into pool, and check for race condition
                VDOMNode * fromPool = document->getNodePool().put(node,result);
                if (fromPool != NULL && fromPool!=result) {

                    delete result;
                    result = fromPool;

                }
            }
            break;
        default:
            result = NULL;
            break;

    }

    return result;
}

void DOMNode::assertLIBXML2VDOM(VDOMNode * node) throw (invalid_argument) {


    if (!strstr(typeid(*node).name(),"LIBXML2")) {

        stringstream ss;
        ss << "Provided VDOMNode* is not part of LIBXML2 implementation, cannot proceeed";
        throw invalid_argument(ss.str());

     }

}

xmlNodePtr DOMNode::getNodePointer() {

    return this->nodePtr;
}

VDOMDocument * DOMNode::getOwnerDocument() {

    return this->document;

}

VDOMNodeList * DOMNode::getChildNodes() {

    VDOMNodeList* nodeList = new VDOMNodeList();

    //-- Foreach children linked list
    for (xmlNode * currentChild = this->nodePtr->children;currentChild;currentChild=currentChild->next) {

        VDOMNode * node = DOMNode::toNode(currentChild,this->document);
        if (node!=NULL)
            nodeList->push_back(node);

    }

    return nodeList;

}

VDOMNode * DOMNode::getParentNode() {

    //-- If no parent or no local pointer -> return NULL
    if (this->nodePtr==NULL || this->nodePtr->parent == NULL )
        return NULL;

    //-- Return result from toNode
    return DOMNode::toNode(this->nodePtr->parent,this->document);


}



} /* namespace LIBXML2 */
} /* namespace VDOM */
