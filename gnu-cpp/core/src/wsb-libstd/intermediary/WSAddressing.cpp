/*
 * WSAddressing.cpp
 *
 *  Created on: TODO
 *  Author: rleys
 */

// Includes
//----------------

//-- Std
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/broker/IntermediaryException.h>

//-- Message
#include <wsb-libstd/message/soap/SOAPMessage.h>

//-- Broker
#include <wsb-core/broker/Intermediary.h>
#include <wsb-core/broker/IntermediaryFactory.h>

//-- XML
#include <vxml/vdom/VDOMElement.h>

#include "WSAddressing.h"

namespace OSI {
namespace WSB {


WSAddressing::WSAddressing() {


}

WSAddressing::~WSAddressing() {

}

string WSAddressing::getSupportedIntermediaryName() {

    return "wsa";

}


Intermediary * WSAddressing::createIntermediary() {

    return new WSAddressing();

}


void WSAddressing::down(MessageContext * context) throw (IntermediaryException) {




    // Extract wsa:Action as Qualifier
    //------------------------------
    SOAPMessage * soap = dynamic_cast<SOAPMessage*>(context->getMessage());
    soap->addXPathNamespace("wsa",NS_WSA);
    VDOM::VDOMElement * actionElement = soap->getHeaderElement("wsa:Action");

    //-- Fail if nothing
    if (actionElement==NULL) {

        stringstream ss;
        ss << "The input message did not have a S:Header/wsa:Action to extract qualifier. This is required if the WS-Addressing Intermediary is in the tree";

        Logging::getLogger("libstd.intermediary.wsa")->errorStream() << ss.str();

        throw ss.str();

    } else if (actionElement->getTextContent().length()==0) {

        stringstream ss;
        ss << "The input message did not have a S:Header/wsa:Action to extract qualifier. This is required if the WS-Addressing Intermediary is in the tree";

        Logging::getLogger("libstd.intermediary.wsa")->errorStream() << ss.str();

        //throw IntermediaryException(ss.str());

        throw ss.str();

    }
    //-- Record as Qualifier
    else {

        context->getMessage()->setQualifier(actionElement->getTextContent());

    }

}

/// To be Implemented
void WSAddressing::up(MessageContext * context) throw (IntermediaryException) {


    Logging::getLogger("libstd.intermediary.wsa")->infoStream() << "up";

	// Add MessageID
	//--------------------

	//-- Register NS
	SOAPMessage * soap = dynamic_cast<SOAPMessage*>(context->getMessage());
	soap->addXPathNamespace("wsa",NS_WSA);

	//-- Generate a random ID
	srand(time(NULL));

	//-- Add The MessageID
	stringstream ss;
	ss << rand();
	soap->addHeader(NS_WSA,"wsa:MessageID",ss.str());

	// FIXME Add RelatesTo
	//--------------------------

	// Add Action if there is a qualifier
	//----------------------------------------
	if(soap->getQualifier().length()>0) {

	    Logging::getLogger("libstd.intermediary.wsa")->infoStream() << "Adding Action header";

	    soap->addHeader(NS_WSA,"wsa:Action",soap->getQualifier());

	}



}



} // EOF Namespace OSI
} // EOF Namespace WSB

