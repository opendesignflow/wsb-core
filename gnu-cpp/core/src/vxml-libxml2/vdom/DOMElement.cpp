/**
 * DOMElement.cpp
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
#include <cstring>
using namespace std;

//-- Libxml2 VDOM
#include "DOMNode.h"
#include "DOMElement.h"


namespace VDOM {
namespace LIBXML2 {

DOMElement::DOMElement(xmlNodePtr nodePrt,DOMDocument * ownerDocument): DOMNode(nodePrt,ownerDocument) {


}

DOMElement::~DOMElement() {
    // TODO Auto-generated destructor stub
}



VDOMNode * DOMElement::cloneNode(bool recursive) {

    return DOMNode::cloneNode(recursive);

}

string DOMElement::getLocalName() {


    return string((const char*)this->nodePtr->name);

}


string DOMElement::getName() {

    //-- If NS: Add Prefix to name if necessary, otherwise return name
    if (this->nodePtr->ns!=NULL) {
        stringstream ss;

        //-- Add Prefix if one
        if (this->nodePtr->ns->prefix!=NULL)
            ss << (const char*)this->nodePtr->ns->prefix << ":";

        //-- Add Name now
        ss << (const char*)this->nodePtr->name;

        return ss.str();
    } else
        return string((const char*)this->nodePtr->name);

}


string DOMElement::getNamespaceURI() {

    return (this->nodePtr->ns==NULL) ? "" : string((const char*)this->nodePtr->ns->href);

}


string DOMElement::getPrefix() {


    return (this->nodePtr->ns==NULL || this->nodePtr->ns->prefix==NULL  ) ? "" : string((const char*)this->nodePtr->ns->prefix);
}

VDOMNodeList* DOMElement::getElementsByTagName(string name) {


    VDOMNodeList* nodeList = new VDOMNodeList();


    //-- Foreach children linked list
    for (xmlNode * currentChild = this->nodePtr->children;currentChild;currentChild=currentChild->next) {

        //-- If Element and matching Tag name
        if (currentChild->type == XML_ELEMENT_NODE && strcmp((const char*)currentChild->name,name.c_str())==0 ) {

            nodeList->push_back(dynamic_cast<VDOMElement*>(DOMNode::toNode(currentChild,this->document)));
        }

    }


    return nodeList;

}


VDOMNodeList* DOMElement::getElementsByTagNameNS(string namespaceURI,string localname) {

    VDOMNodeList* nodeList = new VDOMNodeList();

    //-- Foreach children linked list
    for (xmlNode * currentChild = this->nodePtr->children;currentChild;currentChild=currentChild->next) {

        //-- If Element and matching Tag name + namespace URI
        if (currentChild->type == XML_ELEMENT_NODE
                && currentChild->ns!=NULL
                && strcmp((const char*)currentChild->ns->href,namespaceURI.c_str())
                && strcmp((const char*)currentChild->name,localname.c_str())==0 ) {

            nodeList->push_back(dynamic_cast<VDOMElement*>(DOMNode::toNode(currentChild,this->document)));
        }

    }

    return nodeList;

}

bool DOMElement::hasAttribute(string name) {

    xmlAttrPtr attributePtr =  xmlHasProp(this->getNodePointer(),(const xmlChar*)name.c_str());
    return attributePtr != NULL ? true : false;

}


string DOMElement::getAttribute(string name) {


    xmlChar * attributeValue = xmlGetProp(this->nodePtr,(const xmlChar*)name.c_str());
    if (attributeValue) {
        string res((const char*)attributeValue);
        xmlFree(attributeValue);
        return res;
    }
    return string();
}

void DOMElement::setAttribute(string name,string value) {


    //-- If value is empty, and attribute is present -> remove it
    xmlAttrPtr attributePtr =  xmlHasProp(this->getNodePointer(),(const xmlChar*)name.c_str());
    if (value.size() == 0 && attributePtr!=NULL)  {
        xmlRemoveProp(attributePtr);
    }
    //-- Value not empty -> set attribute value
    else if (value.size()>0) {
        xmlSetProp(this->getNodePointer(),(const xmlChar*)name.c_str(),(const xmlChar*)value.c_str());
    }



}


VDOMDocument * DOMElement::getOwnerDocument() {
    return DOMNode::getOwnerDocument();
}

void DOMElement::appendChild(VDOMNode * newChild) {

    //-- Check Libxml2 implementation
    DOMNode::assertLIBXML2VDOM(newChild);

    //-- Do
    xmlAddChild(this->nodePtr,dynamic_cast<DOMNode*>(newChild)->getNodePointer());

}

VDOMNodeList * DOMElement::getChildNodes() {

    return DOMNode::getChildNodes();

}

/**
 * @see VDOM::VDOMNode
 * @return
 */
VDOMNode * DOMElement::getParentNode() {

    return DOMNode::getParentNode();

}

void DOMElement::setTextContent(string& content) {


    xmlNodeSetContent(this->getNodePointer(),(const xmlChar*)content.c_str());
}


string DOMElement::getTextContent() {

   const char * content = (const char *) xmlNodeGetContent(this->getNodePointer());
   if (!content)
       return string();
   else
       return string(content);
}

void DOMElement::insertBefore(VDOMElement * newNode,VDOMElement * childNode) {

    //-- Checks
    DOMNode::assertLIBXML2VDOM(newNode);
    DOMNode::assertLIBXML2VDOM(childNode);

    //-- Add as prev sibling
    xmlAddPrevSibling(dynamic_cast<DOMElement*>(childNode)->getNodePointer(),dynamic_cast<DOMElement*>(newNode)->getNodePointer());



}

} /* namespace LIBXML2 */
} /* namespace VDOM */
