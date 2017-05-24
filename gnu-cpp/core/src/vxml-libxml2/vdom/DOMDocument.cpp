/**
 * DOMDocument.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <iostream>
using namespace std;

//-- VDOM
#include <vxml/vdom/VDOMElement.h>

//-- Lib XML2
#include <libxml2/libxml/parser.h>

#include "../vxpath/XPathContext.h"
#include "DOMDocument.h"

namespace VDOM {
namespace LIBXML2 {

DOMDocument::DOMDocument(xmlDocPtr doc) : DOMNode(NULL,this) {

    //-- Record pointer
    this->doc = doc;

}

DOMDocument::~DOMDocument() {

    //-- Clear Node pool
    map<xmlNodePtr,VDOMNode*>::iterator it;
    for (it = this->nodePool.getMap().begin() ; it != this->nodePool.getMap().end();it++) {
        delete (*it).second;
    }

    //-- Free Document pointer
    xmlFreeDoc(this->doc);

}


xmlDocPtr& DOMDocument::getDocumentPointer() {
    return this->doc;
}

NodePool<xmlNodePtr>& DOMDocument::getNodePool() {
    return this->nodePool;
}

VDOMNode * DOMDocument::cloneNode(bool recursive) {

    return DOMNode::cloneNode(recursive);

}
VXPathContext * DOMDocument::createXPathContext(VDOMElement * baseElement) {

    //-- Ensure the provided baseElement is of our implementation
    DOMElement * implElement = NULL;
    if (baseElement!=NULL) {
        DOMNode::assertLIBXML2VDOM(baseElement);
        implElement = dynamic_cast<DOMElement*>(baseElement);
    }

    //-- Create LIBXML2 Implementation of VXPathContext for the moment
    if (implElement!=NULL)
        return new XPathContext(implElement);
    else
        return new XPathContext(this);



}

VDOMElement * DOMDocument::getDocumentElement() {

    //-- Get Document Element from libxml2
    xmlNodePtr documentElement = xmlDocGetRootElement(this->doc);

    //cout << "Document element: " << documentElement << endl;

    if (documentElement!=NULL)
        return dynamic_cast<VDOMElement*>(DOMNode::toNode(documentElement,this));
    return NULL;
}


VDOMElement * DOMDocument::createElementNS(string& nsURI,string&qname) {

    //-- Parse NS to determine prefix
    size_t prefixEndPos = qname.find_first_of(':');
    string prefix = prefixEndPos == string::npos ? "": qname.substr(0,prefixEndPos);

    //-- Create NS
    xmlNsPtr nsPtr = xmlNewGlobalNs(this->doc,(const xmlChar*)nsURI.c_str(),(const xmlChar*)prefix.c_str());

    //-- Create Node
    xmlNodePtr nodePtr = xmlNewDocRawNode(this->doc,nsPtr,(const xmlChar*)qname.c_str(),NULL);

    return dynamic_cast<VDOMElement*>(DOMNode::toNode(nodePtr,this));
}


VDOMElement * DOMDocument::createElement(string& name) {

    //-- Create Node
    xmlNodePtr nodePtr = xmlNewDocRawNode(this->doc,NULL,(const xmlChar*)name.c_str(),NULL);

    return dynamic_cast<VDOMElement*>(DOMNode::toNode(nodePtr,this));

}

VDOMDocument * DOMDocument::getOwnerDocument() {
    return DOMNode::getOwnerDocument();
}

void DOMDocument::appendChild(VDOMNode * newChild) {

    //-- Check Libxml2 implementation
    DOMNode::assertLIBXML2VDOM(newChild);

    //-- No Root element -> Add as root element
    xmlNodePtr rootElement = xmlDocGetRootElement(this->document->getDocumentPointer());
    if (rootElement==NULL) {
        xmlDocSetRootElement(this->document->getDocumentPointer(),dynamic_cast<DOMNode*>(newChild)->getNodePointer());
    }
    //-- Add as First Child of root
    else {
        xmlAddChild(rootElement,dynamic_cast<DOMNode*>(newChild)->getNodePointer());
    }
}

VDOMNodeList * DOMDocument::getChildNodes() {

    return DOMNode::getChildNodes();

}


VDOMNode * DOMDocument::getParentNode() {
    return DOMNode::getParentNode();
}

} /* namespace LIBXML2 */
} /* namespace VDOM */
