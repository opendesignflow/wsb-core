/*
 * Broker.cpp
 *
 *  Created on: Nov 14, 2011
 *      Author: rleys
 */

// Includes
//---------------------

//-- Std
#include <fstream>
using namespace std;


//-- Common
#include <wsb-core/common/Logging.h>

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Network
#include <wsb-core/network/NetworkLayer.h>

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>
#include <wsb-core/broker/MessageBroker.h>

#include "Broker.h"


namespace OSI {
namespace WSB {

Broker::Broker() {

	//-- Defaults
	this->brokeringTree = NULL;

}

Broker::~Broker() {

	//-- Delete The brokering tree
	delete brokeringTree;

}

void Broker::broker(NetworkLayer * networkLayer,MessageContext * messageContext) {

    //-- Don't broker if there is no tree
    if (this->brokeringTree==NULL) {
        Logging::getLogger("broker")->errorStream() << "No brokering tree in Broker: Cannot broker";
        return;
    }

	//-- Create a Message Broker instance
	MessageBroker * messageBroker = new MessageBroker(networkLayer,messageContext,*(this->brokeringTree));

	//-- Submit it to thread queue
	Logging::getLogger("broker")->infoStream() << "Dispatching Message Broker";
	messageBroker->dispatchAndWait(&(this->workqueue));

	// Response
	//--------------------------
	if (messageContext->getResponseContext()!=NULL && messageContext->getDirection()==MessageContext::DOWN) {

	    //-- Set Network Context on message context
	    MessageContext * responseContext = messageContext->getResponseContext();
	    responseContext->setDirection(MessageContext::UP);

	    //-- Up
	    MessageBroker * responseBroker = new MessageBroker(networkLayer,responseContext,*(this->brokeringTree));
	    responseBroker->dispatchAndWait(&(this->workqueue));

	    //-- Send
	    //networkLayer->send(responseContext);

	}


}

void Broker::send(NetworkLayer * networkLayer,MessageContext * messageContext) {

	//-- Ensure we are brokering UP
	messageContext->up();

	//-- Broker
	this->broker(networkLayer,messageContext);


}


BrokeringTree * Broker::getBrokeringTree() {

    return this->brokeringTree;
}

void Broker::setBrokeringTree(BrokeringTree* newTree) {

    //this->lifecycleAssert(LIFECYCLE_INITIAL,"setBrokeringTree");

    if (this->brokeringTree!=NULL) {
        delete this->brokeringTree;
    }
    this->brokeringTree = newTree;

}


void Broker::readInFromString(string xml) {

	this->lifecycleAssertNot(LIFECYCLE_STARTED);

	//-- Delete the brokering tree if there is one
	if (this->brokeringTree!=NULL)
			delete this->brokeringTree;

	//-- Create and reading
	this->brokeringTree = new BrokeringTree();
	this->brokeringTree->readInFromString(xml);
}


void Broker::readInFromFile(string filepath) {

	this->lifecycleAssertNot(LIFECYCLE_STARTED);

	//-- Delete the brokering tree if there is one
	if (this->brokeringTree!=NULL)
			delete this->brokeringTree;

	//-- Get File
	std::ifstream t(filepath);
	if (t.bad()) {
		Logging::getLogger("wsb.broker")->warnStream() << "Brokering tree file not found: " << filepath;
		return;
	}

	//-- Get content
	std::string btreeContent((std::istreambuf_iterator<char>(t)),
					 std::istreambuf_iterator<char>());

	//-- Create and reading
	this->brokeringTree = new BrokeringTree();
	this->brokeringTree->readInFromString(btreeContent);

}


void Broker::init() {



	//-- (Re)-Init the Brokering Tree
	if (this->brokeringTree!=NULL) {
		delete this->brokeringTree;
		this->brokeringTree = NULL;
	}

	Lifecycle::init();

}

void Broker::start() {

	//-- Start dispatch queue
	this->workqueue.start();

	Lifecycle::start();
}

void Broker::reload() {
    Lifecycle::reload();
}

void Broker::stop() {

    Logging::getLogger("wsb.broker.stop")->warnStream() << "Stopping Broker Requested";

	//-- Stop dispatch queue
	this->workqueue.stop();
	this->workqueue.join();

	Lifecycle::stop();

	Logging::getLogger("wsb.broker.stop")->warnStream() << "Broker Stopped";

}

void Broker::finish() {

	//-- Finish dispatch queue
	this->workqueue.finish();

	//-- Clean
	if (this->brokeringTree!=NULL) {
		delete this->brokeringTree;
		this->brokeringTree = NULL;
	}

	Lifecycle::finish();
}

} /* namespace WSB */
} /* namespace OSI */
