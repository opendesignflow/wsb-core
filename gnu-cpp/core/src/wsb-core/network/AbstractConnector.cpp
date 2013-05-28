/*
 * AbstractConnector.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/URI.h>
#include <wsb-core/common/Logging.h>

//-- Sync
#include <pthread.h>
#include <semaphore.h>

//-- Dispatch
#include <wsb-core/common/dispatch/SafeBlockingQueue.h>

//-- Message
#include <wsb-core/message/Message.h>
#include <wsb-core/message/MessageContext.h>
#include <wsb-core/message/MessageFactory.h>

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>

//-- Network
#include <wsb-core/network/ConnectorException.h>

#include "AbstractConnector.h"

namespace OSI {
namespace WSB {

AbstractConnector::AbstractConnector(string uri) {

	//-- Record URI
	this->uri = new URI(uri);
	if (!this->uri->isValid()) {
	    throw ConnectorException("Provided URI "+uri+" is not valid");
	}

	//-- Message Queue
	this->messages = new SafeBlockingQueue<MessageContext*>();

	//-- Init variables
	this->brokerTree = NULL;

	//-- Default direction: Server
	this->connectorDirection = AbstractConnector::SERVER;
}

AbstractConnector::~AbstractConnector() {

    //-- Clean pointer based things
	delete this->uri;
}

URI* 	AbstractConnector::getURI() {
	return this->uri;
}


string 	AbstractConnector::getMessageType() {
	return this->messageType;
}


void 	AbstractConnector::setMessageType(string messageType) {
	this->messageType = messageType;
}

BrokeringTree * AbstractConnector::getBrokeringTree() {

	return this->brokerTree;

}

void AbstractConnector::setBrokeringTree(BrokeringTree * brokerTree) {
	this->brokerTree = brokerTree;
}


AbstractConnector::DIRECTION AbstractConnector::getDirection() {
    return this->connectorDirection;
}


void AbstractConnector::setDirection(AbstractConnector::DIRECTION direction) {
    this->connectorDirection = direction;
}

Message * 	AbstractConnector::buildMessage(string messageString) {

    try {

        // Find Factory
        //--------------------
        MessageFactory * factory = MessageFactory::getInstance(this->messageType);
        // Build Message
        //-------------------
        Message * message = factory->build(messageString);


        // Return
        //-------------
        delete factory;

        return message;

    } catch (MessageFactoryNotFound& ex) {

        //-- Log
        Logging::getLogger("network.abstractconnector.buildMessage")->critStream() << "Message building failed for type: "<< this->messageType<<", check you have setup a valid messageType on this connector, and registered a MessageBuilder for this type";

        //-- Forward Throw
        throw ex;

    }

    return NULL;


}

log4cpp::Category * AbstractConnector::getLogger(string baseString) {

    return Logging::getLogger(baseString.append(this->getDirection() == AbstractConnector::SERVER ? ".server" : ".client").c_str());

}

void AbstractConnector::offer(Message * message,NetworkContext * context) {


	//-- Create MessageContext
	MessageContext * messageContext = new MessageContext(message,context);

	//-- Push
	this->messages->offer(messageContext);


}

MessageContext * AbstractConnector::poll(long int waitns) {


	//-- Prepare Return
	MessageContext * msg = this->messages->getBlocking(waitns);


	return msg;
}


void AbstractConnector::send(MessageContext * messageContext) {

    Logging::getLogger("network.abstractconnector")->infoStream() << "Sending message context " << messageContext ;


	//-- Call maintain connection
	this->maintainConnection(messageContext->getNetworkContext());

	//-- Do the send
	this->doSend(messageContext);

}


void AbstractConnector::init() {
	Lifecycle::init();
}

void AbstractConnector::stop() {
    Lifecycle::stop();
	Thread::stop();
}


void AbstractConnector::start() {

	//-- If not inited already, call
	Lifecycle::lifecycleAssert(LIFECYCLE_STATE::LIFECYCLE_STOPPED,"AbstractConnector/start()");

	//-- Call Thread
	Thread::start();
}

/**
 * Call Redirection to Thread
 */
void AbstractConnector::finish() {
	Thread::finish();
}


} // namespace WSB
} // namespace OSI
