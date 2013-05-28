/*
 * SOAPMessageFactory.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//--------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/xml/DOMManipulator.h>

//-- Message
#include <wsb-core/message/MessageFactory.h>
#include <wsb-core/message/Message.h>
#include <wsb-core/message/MessageBuilderException.h>

#include <wsb-libstd/message/soap/SOAPMessage.h>

//-- XML
#include <wsb-core/common/xml/XMLBuilderFactory.h>
#include <wsb-core/common/xml/XMLBuilder.h>


#include "SOAPMessageFactory.h"

namespace OSI {
namespace WSB {

SOAPMessageFactory::SOAPMessageFactory() {

	//-- Init Xerces, which we need
	//XQillaPlatformUtils::initialize();

	/*try {
		XMLPlatformUtils::Initialize(); // Initialize Xerces infrastructure
	} catch (XMLException& e) {
		char* message = XMLString::transcode(e.getMessage());
		cerr << "XML toolkit initialization error: " << message << endl;
		XMLString::release(&message);
		// throw exception here to return ERROR_XERCES_INIT
	}*/


}

SOAPMessageFactory::~SOAPMessageFactory() {
	// TODO Auto-generated destructor stub
}


SOAPMessage * SOAPMessageFactory::build(string message) throw (MessageBuilderException) {

	//-- Create a SOAP Message


	// Read with DOM
	//----------------------
	Logging::getLogger("message.soap.factory")->debug("Building SOAP Message: Parsing XML with DOM");


	try {
		//-- Parse
		XMLBuilder * builder = XMLBuilderFactory::getInstance()->getBuilder();
		VDOM::VDOMDocument *  document =  builder->parseString(message);

		DOMManipulator * manipulator = new DOMManipulator(document);

		//-- Prepare NS
		manipulator->getNSResolver()["S"] = "http://www.w3.org/2001/12/soap-envelope";


		//-- Check Envelope / Header / Body
		bool baseStructure = manipulator->hasElement("/S:Envelope/S:Header")
								&& manipulator->hasElement("/S:Envelope/S:Body");

	//	bool baseStructure = false;

		Logging::getLogger("message.soap.factory")->debugStream() << "Structure check: " << baseStructure;

		if (!baseStructure) {

			//-- Throw Message Exception
			MessageBuilderException ex("Message doesn't conform to SOAP base structure");
			throw ex;
		}

		// Create SOAP Message and return
		//---------------------------------------

		//-- Create Message
		SOAPMessage * soapMessage = new SOAPMessage(manipulator);

		return soapMessage;

	} catch (XMLBuilderException& e) {

		//-- Error Occured while building
		/*Logging::getLogger("message.soap.factory")->critStream()
					<< "Error while parsing SOAP XML document: "
					<< UTF8(domError.getMessage())
					<< " @-> " << domError.getRelatedException();*/

		//-- Throw Message Exception
		MessageBuilderException ex("Error while parsing SOAP XML document: ");
		ex << e.what();
		throw ex;
	}

	return NULL;


}

SOAPMessage * SOAPMessageFactory::build() throw (MessageBuilderException) {
	return new SOAPMessage();
}


} /* namespace WSB */
} /* namespace OSI */
