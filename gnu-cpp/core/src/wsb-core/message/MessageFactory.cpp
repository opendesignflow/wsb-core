/*
 * MessageFactory.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//--------------

//-- Std
#include <string>
#include <sstream>
using namespace std;

//-- Message
#include <wsb-core/message/Message.h>
#include <wsb-core/message/MessageFactoryNotFound.h>
#include <wsb-libstd/message/soap/SOAPMessageFactory.h>

#include "MessageFactory.h"

namespace OSI {
namespace WSB {

MessageFactory::MessageFactory() {
	// TODO Auto-generated constructor stub

}

MessageFactory::~MessageFactory() {
	// TODO Auto-generated destructor stub
}

MessageFactory  * MessageFactory::getInstance(string messageType) throw (MessageFactoryNotFound) {

	MessageFactory * factory = NULL;

	// SOAP
	//------------------
	if (messageType == "soap") {
		factory = new SOAPMessageFactory();
	} else {
	    stringstream ss;
	    ss << "Message Factory Not found for message type: " << messageType;

		//MessageFactoryNotFound m("Message Factory Not found for message type: "+messageType);
		//throw ss.str();
		throw MessageFactoryNotFound(ss.str());
	}


	return factory;

}

} /* namespace WSB */
} /* namespace OSI */
