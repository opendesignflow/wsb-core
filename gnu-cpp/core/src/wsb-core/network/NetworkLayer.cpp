/*
 * NetworkLayer.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

// Includes
//-----------------------

//-- Std
#include <iterator>
#include <list>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/URI.h>

//-- Broker
#include <wsb-core/broker/Broker.h>

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Network
#include <wsb-core/network/AbstractConnector.h>
#include <wsb-core/network/MessagePoller.h>

#include "NetworkLayer.h"

namespace OSI {
namespace WSB {

NetworkLayer::NetworkLayer() {

	//-- Create The first default poller
	this->pollers.push_back(MessagePoller(this));

	//-- Init
	this->broker = NULL;
}

NetworkLayer::~NetworkLayer() {
}

void NetworkLayer::send(MessageContext * ctx) {

	//- Try to fins out which connector should be called
	if (ctx->getNetworkContext()!=NULL) {

	    //-- Connector URI
	    stringstream connectorURI;
        connectorURI << ctx->getNetworkContext()->getURI()->getScheme() << "://" << ctx->getNetworkContext()->getURI()->getHost();

		//-- look in map
		AbstractConnector * connector = this->connectors[connectorURI.str()];
		if (connector!=NULL) {

			//-- Submit to output Queue
			Logging::getLogger("network")->infoStream() << "Found a connector to send message";
			connector->send(ctx);

		} else {
			Logging::getLogger("network")->warnStream() << "No Connector found for sending using URI: " << ctx->getNetworkContext()->getURI()->getURI();
		}



	} else  {

		Logging::getLogger("network")->infoStream() << "No network context to send message";

	}

}


void NetworkLayer::addConnector(AbstractConnector * connector) {

	//-- Ensure we are not started
	this->lifecycleAssertNot(LIFECYCLE_STARTED);

	//-- Register into map
	stringstream connectorURI;
	connectorURI << connector->getURI()->getScheme() << "://" << connector->getURI()->getHost();
	this->connectors[connectorURI.str()]=connector;

	//-- Assign to poller 0
	this->pollers.front().connect(connector);

	//-- Ensure connector is inited
	if (connector->getLifecycleState()==LIFECYCLE_INITIAL) {
	    connector->init();
	}

}

Broker * NetworkLayer::getBroker() {
	return this->broker;
}


void NetworkLayer::setBroker(Broker * broker) {

	//-- Ensure we are not started
	this->lifecycleAssertNot(LIFECYCLE_STARTED);

	this->broker = broker;

}


void NetworkLayer::init() {

	//-- Ensure all pollers have the Broker
	list<MessagePoller>::iterator mit;
	for (mit = this->pollers.begin();mit != this->pollers.end(); mit++) {

		//-- Give Broker
		mit->setBroker(this->broker);

		//-- Init
		mit->init();
	}

	//-- Ensure all connector are init
	map<string,AbstractConnector*>::iterator cit;
    for (cit = this->connectors.begin();cit != this->connectors.end(); cit++) {

        //-- Init if still in initial state
        if ((*cit).second->getLifecycleState()==LIFECYCLE_INITIAL)
            (*cit).second->init();

    }

    Lifecycle::init();

}

/**
 * Start the network Layer
 */
void NetworkLayer::start() {

	//-- Start Pollers
	list<MessagePoller>::iterator mit;
	for (mit = this->pollers.begin();mit != this->pollers.end(); mit++) {

		mit->start();
	}


	//-- Start Connectors
	map<string,AbstractConnector*>::iterator cit;
	for (cit = this->connectors.begin();cit != this->connectors.end(); cit++) {

	    // Don't start client ones
	    //if ((*cit).second->getDirection()==AbstractConnector::SERVER)
	        (*cit).second->start();

	}

	Lifecycle::start();


}


void NetworkLayer::stop() {

    Logging::getLogger("wsb.network.stop")->infoStream() << "Stopping Connectors";

	//-- Stop Connectors
	map<string,AbstractConnector*>::iterator cit;
	for (cit = this->connectors.begin();cit != this->connectors.end(); cit++) {

	    // Stop and wait stopped
		(*cit).second->stop();
		(*cit).second->join();

	}

	Logging::getLogger("wsb.network.stop")->infoStream() << "Stopping Message Pollers";

	//-- Stop Pollers
	list<MessagePoller>::iterator mit;
	for (mit = this->pollers.begin();mit != this->pollers.end(); mit++) {

	    // Stop and wait stopped
		mit->stop();
		mit->join();
	}



	Lifecycle::stop();

	Logging::getLogger("wsb.network.stop")->infoStream() << "Stopped";

}



void NetworkLayer::reload() {

}

void NetworkLayer::finish() {

    //-- Finish Connectors
    map<string,AbstractConnector*>::iterator cit;
    for (cit = this->connectors.begin();cit != this->connectors.end(); cit++) {

        (*cit).second->finish();

    }

    //-- Finish Pollers
    list<MessagePoller>::iterator mit;
    for (mit = this->pollers.begin();mit != this->pollers.end(); mit++) {

        mit->finish();
    }

    Lifecycle::finish();

}


} /* namespace WSB */
} /* namespace OSI */
