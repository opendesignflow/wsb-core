/**
 * XPathContext.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <stdexcept>
#include <sstream>
using namespace std;



//-- LibXML2 impl


#include "XPathContext.h"

namespace VDOM {
namespace LIBXML2 {

XPathContext::XPathContext(VDOM::LIBXML2::DOMDocument * document) {

    this->xpathCtx = xmlXPathNewContext(document->getDocumentPointer());
    this->document = document;
}

XPathContext::XPathContext(VDOM::LIBXML2::DOMElement * element) {



    this->xpathCtx = xmlXPathNewContext(element->getNodePointer()->doc);
    this->xpathCtx->node = element->getNodePointer();
    this->document = dynamic_cast<DOMDocument*>(element->getOwnerDocument());
}


XPathContext::~XPathContext() {
    xmlXPathFreeContext(this->xpathCtx);
}

void XPathContext::addNamespaceBinding(string prefix,string nsuri) {



    if(xmlXPathRegisterNs(this->xpathCtx,(const xmlChar*) prefix.c_str(), (const xmlChar*)nsuri.c_str()) != 0) {

        stringstream ss;
        ss << "Registering namespace "<<prefix<<":{"<<nsuri<<"} failed";
        throw invalid_argument(ss.str());
    }

}

VXPathResult * XPathContext::evaluate(string xpath) throw (invalid_argument) {




    //-- Evaluate
    xmlXPathObjectPtr results = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(),this->xpathCtx);
    if (results == NULL) {

        //-- Return error
        stringstream ss;
        ss << "Could not evaluate XPATH expression /"<<xpath<<"/ failed: " << (this->xpathCtx->lastError.message);
        throw invalid_argument(ss.str());

    }

    //-- Fill up results
    //--------------------
    XPathResult * result = new XPathResult();
    unsigned int nodesetSize = results->nodesetval ? results->nodesetval->nodeNr : 0;

    for (unsigned int i=0;i<nodesetSize ; i++) {

        // Get Node Pointer
        xmlNodePtr node = results->nodesetval->nodeTab[i];

        // Convert to Object
        VDOMNode * nodeObject = DOMNode::toNode(node,this->document);
        if (nodeObject!=NULL)
            result->getNodeList().push_back(nodeObject);

    }

    //-- Free
    xmlXPathFreeObject(results);

    return result;

}

string XPathContext::evaluateAsString(string xpath) throw (invalid_argument)  {

    //-- Evaluate
    xmlXPathObjectPtr results = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(),this->xpathCtx);
    if (results == NULL) {

        //-- Return error
        stringstream ss;
        ss << "Could not evaluate XPATH expression /"<<xpath<<"/ failed: " << (this->xpathCtx->lastError.message);
        throw invalid_argument(ss.str());

    }

    //-- Determine Result:
    //-- * Take first result if one, and infer a string if its an element or an attribute
    unsigned int nodesetSize = results->nodesetval ? results->nodesetval->nodeNr : 0;
    string result;
    if (nodesetSize>0) {

        //-- Switch on first result type
        xmlNodePtr firstResult =  results->nodesetval->nodeTab[0];
        char* nodeContent = NULL;
        switch(firstResult->type) {

            case XML_ATTRIBUTE_NODE:
            case XML_ELEMENT_NODE:

                nodeContent = (char*)xmlNodeGetContent(firstResult);
                if (nodeContent!=NULL) {
                    result.append(nodeContent);
                    free(nodeContent);
                }
                break;
           /* case XML_ATTRIBUTE_NODE:

                result.append((const char*)(firstResult)->children->content);
                break;*/

            default:
                break;

        }

    }

    //-- Free
    xmlXPathFreeObject(results);

    return result;
}


XPathContext::XPathResult::XPathResult() {

}

XPathContext::XPathResult::~XPathResult() {

}


} /* namespace LIBXML2 */
} /* namespace VDOM */
