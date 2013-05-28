/*
 * MessageContext.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std

//-- Broker
#include <wsb-core/broker/IntermediaryException.h>

//-- Message
#include <wsb-core/message/MessageFactory.h>
#include <wsb-core/message/Message.h>

//-- Network
#include <wsb-core/network/NetworkContext.h>


#include "MessageContext.h"

namespace OSI {
namespace WSB {

MessageContext::MessageContext(Message * message) {

	//-- Init
	this->message 			= message;
	this->networkContext 	= NULL;
	this->messageDirection	= MessageContext::DOWN;
	this->error             = NULL;
	this->responseContext = NULL;

}

MessageContext::MessageContext(Message * message,NetworkContext * networkContext){
	this->message 			= message;
	this->networkContext 	= networkContext;
	this->messageDirection  = MessageContext::DOWN;
    this->error             = NULL;
    this->responseContext = NULL;
}

MessageContext::~MessageContext() {

}

Message * MessageContext::getMessage() {
	return this->message;
}

NetworkContext * MessageContext::getNetworkContext() {
	return this->networkContext;
}


IntermediaryException * MessageContext::getError() {
    return this->error;
}


void MessageContext::setError(IntermediaryException exception) {

    //-- Copy
    this->error = new IntermediaryException(exception);

}


void MessageContext::loop() {

    //-- Reset Message
    //-----------------------

    //-- Recreate a message
    this->message = MessageFactory::getInstance(this->message->getType())->build();

}


/// Set the message to be upped
void MessageContext::up() {
	this->messageDirection = MessageContext::UP;
}

/// Set the message to be downed
void MessageContext::down() {
	this->messageDirection = MessageContext::DOWN;
}

MessageContext::DIRECTION MessageContext::getDirection() {
	return this->messageDirection;
}

void MessageContext::setDirection(MessageContext::DIRECTION direction) {
    this->messageDirection = direction;;
}

MessageContext * MessageContext::getResponseContext() {
    return this->responseContext;
}

void MessageContext::setResponseContext(MessageContext *responseContext) {
    this->responseContext = responseContext;

}


} /* namespace WSB */
} /* namespace OSI */
