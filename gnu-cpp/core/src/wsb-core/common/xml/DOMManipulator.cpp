/*
 * DOMManipulator.cpp
 *
 *  Created on: Nov 9, 2011
 *      Author: rleys
 */

// Includes
//----------------
//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/SGC.h>
#include <wsb-core/common/Logging.h>

//-- XML
#include <wsb-core/common/xml/XMLBuilderFactory.h>

//-- VDOM
#include <vxml/vdom/VDOMDocument.h>
#include <vxml/vxpath/VXPathNSContext.h>

#include "DOMManipulator.h"

namespace OSI {
namespace WSB {

DOMManipulator::DOMManipulator() {

    //-- Create Document
    LGC<XMLBuilder> builder = XMLBuilderFactory::getInstance()->getBuilder();
    this->document = builder->newDocument();


    //-- Prepare NS Resolver
//    this->nsContext = new VDOM::VXPathNSContext();

    this->nsContext["fn"] = "http://www.w3.org/2005/xpath-functions";
    (this->nsContext)["xs"] =  "http://www.w3.org/2001/XMLSchema";

    //-- Prepare Xquery implementation
    //this->xqueyImpl = createXQillaXQCImplementation(XQC_VERSION_NUMBER);
}

DOMManipulator::DOMManipulator(VDOM::VDOMDocument * document) {

    //-- Record document
    this->document = document;

    //-- Prepare NS Resolver
    //this->nsContext = new VDOM::VXPathNSContext();
    //this->nsResolver = new XQillaNSResolver();

    //-- Prepare Xquery implementation
    //this->xqueyImpl = createXQillaXQCImplementation(XQC_VERSION_NUMBER);
}

DOMManipulator::~DOMManipulator() {
    // TODO Auto-generated destructor stub

    //-- release Document and so
    //delete this->nsResolver;
    //delete this->xqueyImpl;
    //this->document->release();
    //this->document = NULL;
    delete this->document;
}

VDOM::VXPathNSContext & DOMManipulator::getNSResolver() {
    return (this->nsContext);
}

VDOM::VDOMDocument * DOMManipulator::getDocument() {
    return this->document;
}

string DOMManipulator::toString() {


    LGC<XMLBuilder> builder = XMLBuilderFactory::getInstance()->getBuilder();
    return builder->toString(this->document);


}

VDOM::VDOMElement * DOMManipulator::addElement(string ns, string qname) {

    //-- Do we have a Document Element
    VDOM::VDOMElement * documentElement = this->document->getDocumentElement();

    //-- Create
    VDOM::VDOMElement * newElement = this->createElement(ns, qname);

    //-- Add as child of DocumentElement or as DocumentElement itself
    if (documentElement == NULL) {
        this->document->appendChild(newElement);
    } else {
        documentElement->appendChild(newElement);
    }

    return newElement;
}

VDOM::VDOMElement * DOMManipulator::addUniqueElement(string ns, string qname,string textContent,VDOM::VDOMNode * base) {

    //-- Select Base
    VDOM::VDOMNode * targetBase = base == NULL ? (
                                    this->document->getDocumentElement() == NULL ?   this->document : (VDOM::VDOMNode*) this->document->getDocumentElement() )
                            : base;


    //-- Try to get element
    VDOM::VDOMElement * newOrExistingElement = this->getElement(qname);

    //-- Create if non existing
    if (newOrExistingElement==NULL) {

        newOrExistingElement = this->createElement(ns,qname,targetBase);
    }

    //-- Update text content
    newOrExistingElement->setTextContent(textContent);

    //-- Return
    return newOrExistingElement;

}

VDOM::VDOMElement * DOMManipulator::createElement(string ns, string qname,VDOM::VDOMNode * base) {

    //-- Select Base
    //VDOM::VDOMNode * targetBase = base == NULL ? this->document : base;

    //-- Create and return
   // try {
        VDOM::VDOMElement * created = this->document->createElementNS(ns, qname);

        //-- Append to base if necessary
        if (base!=NULL)
            base->appendChild(created);

        return created;
    /*} catch (XQillaException& ex) {
        Logging::getLogger("common.xml.DOMManipulator.createElement")->debugStream()
                << "XMLFailed: " << XMLString::transcode(ex.getMessage());
    }*/

    return NULL;

}




VDOM::VDOMElement * DOMManipulator::getElement(string xpath, VDOM::VDOMElement * base) {

    //try {

        //-- Prepare Expression
        LGC<VDOM::VXPathContext> xpathContext = this->document->createXPathContext(base);

        //-- Evaluate
        VDOM::VDOMElement * result = xpathContext->evaluateAsElement(xpath);

        return result;
     // }
   /* } catch (XQillaException& ex) {


        //-- If no results -> It is not bad
        if (ex.getCode() != VDOM::VDOMXPathException::NO_RESULT_ERROR )
            Logging::getLogger("common.xml.DOMManipulator.getElement")->debugStream()
                    << "Get Element failed: " << xpath << " with "
                    << XMLString::transcode(ex.getMessage());
    }*/

    //return NULL;

}

list<VDOM::VDOMElement*> DOMManipulator::getElements(string xpath, VDOM::VDOMElement * base) {

    list<VDOM::VDOMElement*> resultList;

    //try {

        //-- Prepare Expression
        LGC<VDOM::VXPathContext> xpathContext = this->document->createXPathContext(base);

        //-- Evaluate
        LGC<VDOM::VXPathResult>  result = xpathContext->evaluate(xpath);


        //-- Return list
        for (vector<VDOM::VDOMNode*>::iterator it = result->getNodeList().begin(); it!=result->getNodeList().end();it++) {

            //-- Get node
            VDOM::VDOMNode * node = *it;

            //-- Only add if it is a VDOM::VDOMElement
            if (node != NULL && node->getNodeType() == VDOM::VDOMNode::ELEMENT_NODE)
                resultList.push_back(dynamic_cast<VDOM::VDOMElement*>(node));

        }


    /*} catch (XQillaException& ex) {
        Logging::getLogger("common.xml.DOMManipulator.getElements")->debugStream()
                << "Get Elements failed: " << xpath << " with "
                << XMLString::transcode(ex.getMessage());

    }*/

    return resultList;

}

bool DOMManipulator::hasElement(string xpath,VDOM::VDOMElement * base) {

   // try {


        //-- Prepare Expression
        LGC<VDOM::VXPathContext> xpathContext = this->document->createXPathContext(base);

        //-- Evaluate
        return xpathContext->evaluateAsBoolean(xpath);




    /*} catch (VDOM::VDOMXPathException& ex) {

        Logging::getLogger("common.xml.DOMManipulator.hasElement")->critStream()
         << "XPath failed for expression " << xpath << ": " << XMLString::transcode(ex.getMessage());
    }*/

    //return false;
}



string DOMManipulator::getString(string xpath, VDOM::VDOMElement * base) {

    //try {

        //-- Prepare Expression
        LGC<VDOM::VXPathContext> xpathContext = this->document->createXPathContext(base);

        //-- Evaluate
        return xpathContext->evaluateAsString(xpath);


    /*} catch (VDOM::VDOMXPathException& ex) {

        Logging::getLogger("common.xml.DOMManipulator.getString")->debugStream()
                << "XPath failed for expression " << xpath << ": "
                << XMLString::transcode(ex.getMessage());
    }*/

    //return "";

}

int DOMManipulator::getInteger(string xpath, VDOM::VDOMElement * base) {

    //-- Get as String
    string res = this->getString(xpath, base);

    //-- Return
    return strtol(res.c_str(),NULL,0);

}


void DOMManipulator::setAttribute(VDOM::VDOMElement * base,string attributeName,int value) {

    //-- integer to string
    stringstream ss;
    ss << value;

    //-- Set attribute
    this->setAttribute(base,attributeName,ss.str());

}

void DOMManipulator::setAttribute(VDOM::VDOMElement * base,string attributeName,string value) {

    //-- Set attribute
    base->setAttribute(attributeName,value);

}

void DOMManipulator::xquery(string xquery, VDOM::VDOMElement * base) {

    /*XQC_Expression *expr = NULL;
    XQC_Sequence *seq = NULL;
    XQC_DynamicContext *context = NULL;
    XQC_Error err;*/

    //-- Prepare XQuery expression
   /* err = this->xqueyImpl->prepare(this->xqueyImpl, xquery.c_str(), 0, &expr);

    //-- Error -> throw Exception
    if (err!=0) {

        Logging::getLogger("common.xml.DOMManipulator.xquery")->critStream()
                        << "An error occurred while preparing xquery expression: " << xquery;

    }



    //-- Select Base
    //-------------------
    VDOM::VDOMElement * nodeBase = base==NULL ? this->getDocument()->getDocumentElement() : base;




    // Create Query Context
    //--------------------------
    expr->create_context(expr, &context);
    context->

    //-- Bind base to $node
    XQC_Sequence *contextNodeSeq = NULL;
    this->xqueyImpl->create_empty_sequence(this->xqueyImpl,&contextNodeSeq);

    context->set_context_item(context,nodeBase);

    // Execute the query
    //-----------------------------
    err = expr->execute(expr, 0, &seq);
    if (err!=0) {

        Logging::getLogger("common.xml.DOMManipulator.xquery")->critStream()
                        << "An error occurred while executing xquery expression: " << xquery;

    }


    //-- Free
    if (expr!=NULL) expr->free(expr);
    if (seq!=NULL) seq->free(seq);
    if (context!=NULL) context->free(context);;
    //"if ($e/@last-updated) then replace value of node  $e/last-updated with fn:currentDate() else insert node  attribute last-updated {fn:currentDate()} into $e";
    //xquery*/

}

} /* namespace WSB */
} /* namespace OSI */
