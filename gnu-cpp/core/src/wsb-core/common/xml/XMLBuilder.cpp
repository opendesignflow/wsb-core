/*
 * XMLBuilder.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */


// Includes
//---------------

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/SGC.h>

//-- XML
#include <wsb-core/common/xml/XMLBuilderException.h>

//-- VDOM
#include <vxml/vdom/VDOMRegistry.h>

#include "XMLBuilder.h"

namespace OSI {
namespace WSB {


XMLBuilder::XMLBuilder() {

	//-- Init
	//this->parsingException = NULL;

}

XMLBuilder::~XMLBuilder() {

	// TODO Auto-generated destructor stub
	//Logging::getLogger("common.xml.builder.destruct")->infoStream() << "Delete XML Builder";
}


VDOM::VDOMDocument * XMLBuilder::newDocument() throw(XMLBuilderException) {

    //-- Get Implementation
    VDOM::VDOMImplementation * impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Create document
    VDOM::VDOMDocument * doc = impl->newDocument();

    //-- Delete implementation
	delete impl;

	return doc;

}


VDOM::VDOMDocument  * XMLBuilder::parseString(string& xml)  {

    //-- Get Implementation
    VDOM::VDOMImplementation * impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse document
    VDOM::VDOMDocument * doc = impl->parseDocument(xml);

    //-- Delete implementation
    delete impl;

    return doc;
}


string XMLBuilder::toString(VDOM::VDOMDocument * doc) {

    //-- Get Implementation
    VDOM::VDOMImplementation * impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Serialize
    VDOM::VDOMLSSerializer * ser = impl->getSerializer();
    string result = ser->toString(doc);


    //-- Delete implementation and selerialiser
    delete ser;
    delete impl;

    return result;

}

 /*bool XMLBuilder::handleError(const DOMError& domError) {

	//-- Register an exception to fail
	this->parsingException = new XMLBuilderException();
	*(this->parsingException) << "Error while parsing document: " <<  UTF8(domError.getMessage());

	//-- Fail
	return false;
}*/

} /* namespace WSB */
} /* namespace OSI */
