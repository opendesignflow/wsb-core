/*
 * MessagePoller.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- Std
#include <list>
#include <iostream>
#include <errno.h>
using namespace std;

//-- Sync
#include <pthread.h>
#include <semaphore.h>

//-- Common
#include <wsb-core/common/Logging.h>

//-- Broker
#include <wsb-core/broker/Broker.h>
#include <wsb-core/broker/BrokeringTree.h>

//-- Network
#include <wsb-core/network/AbstractConnector.h>
#include <wsb-core/network/NetworkContext.h>
#include <wsb-core/network/NetworkLayer.h>

//-- Message
#include <wsb-core/message/Message.h>
#include <wsb-core/message/MessageContext.h>



#include "MessagePoller.h"

namespace OSI {
namespace WSB {

MessagePoller::MessagePoller(NetworkLayer * networkLayer) {

    this->broker = NULL;
	this->networkLayer = networkLayer;

}

MessagePoller::~MessagePoller() {

}

void MessagePoller::setBroker(Broker * broker) {

	//-- Assert we are not started
	this->lifecycleAssertNot(LIFECYCLE_STARTED);

	this->broker = broker;

}

void MessagePoller::connect(AbstractConnector * connector) {
	this->connectors.push_back(connector);
}

void MessagePoller::run() {


	//-- Starts
	Logging::getLogger("wsb.network.poller")->info("Started Message Poller!!!!");

	// Thread Job
	//---------------
	//-- Sync on stop
	while (!this->stopRequested()) {

		//-- Poll throughout connectors
		//cout << "Started Message Poller!!!!" << endl;
		//

		list<AbstractConnector*>::iterator cit;
		for (cit = this->connectors.begin(); cit != this->connectors.end();cit++) {

			//-- Get a message (don't wait endlessly!)
			MessageContext * messageContext = (*cit)->poll(1000);
			if (messageContext != NULL) {

				//-- We have a message

				// Broker
				//--------------------

				//-- Pass through Connector intermediary chain
				if ((*cit)->getBrokeringTree()!=NULL) {

					(*cit)->getBrokeringTree()->down(messageContext);

				}

				//-- Record NetworkContext in map


				//-- Broker
				if (this->broker!=NULL) {
					// FIXME Broker

					Logging::getLogger("wsb.network.poller")->debugStream() << "Will broker message";

					this->broker->broker(this->networkLayer,messageContext);
				}

			}

		}

	}

	Logging::getInstance()->getLogger("network.poller")->info("Exiting Message Poller!!!!");


}

void MessagePoller::start(){
	Thread::start();
	this->lifecycleProgressToStarted();
}

void MessagePoller::stop(){
	Thread::stop();
	this->lifecycleProgressToStopped();
}

void MessagePoller::finish() {
	Thread::finish();
}

} /* namespace WSB */
} /* namespace OSI */
