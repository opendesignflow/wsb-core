/*
 * LoopbackConnector.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

// Includes
//-------------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/URI.h>

//-- Message
#include <wsb-core/message/MessageBuilderException.h>

//-- Network
#include <wsb-core/network/AbstractConnector.h>

#include "LoopbackConnector.h"

namespace OSI {
namespace WSB {

LoopbackConnector::LoopbackConnector() : AbstractConnector("loopback:"){
	// TODO Auto-generated constructor stub

}

LoopbackConnector::~LoopbackConnector() {
	// TODO Auto-generated destructor stub
}


void LoopbackConnector::loopMessage(string messageString) {

	// Only offer
	//-----------------

	try {
		//-- Build Message
		Message * message = this->buildMessage(messageString);

		//-- Prepare Network Context
		NetworkContext * networkContext = new NetworkContext(this->getURI()->getURI());

		this->offer(message,networkContext);

	} catch (MessageBuilderException& ex) {

		// Message Building failed -> Don't do anything for this connector
		//-----------------
		Logging::getLogger("connector.loopback")->critStream() << ex.what();


	}

}


void LoopbackConnector::run() {

	//-- Do Job :  Nothing

}


void LoopbackConnector::init() {
	// Nothing to do
    AbstractConnector::init();
}


void LoopbackConnector::reload() {

}


void LoopbackConnector::maintainConnection(NetworkContext * networkContext) {

	//-- Always Up :)

}


void LoopbackConnector::doSend(MessageContext * networkContext){

	//-- Serialize
	string serialized = networkContext->getMessage()->toString();


	//-- Offer back
	Logging::getLogger("connector.loopback")->infoStream() << "Received message to send: ";
	Logging::getLogger("connector.loopback")->infoStream() << serialized;
	//this->loopMessage(serialized);

}



} /* namespace WSB */
} /* namespace OSI */
