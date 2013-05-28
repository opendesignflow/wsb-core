/*
 * WSEngine.cpp
 *
 *  Created on: Nov 14, 2011
 *      Author: rleys
 */


// Includes
//----------------------------

//-- Std
#include <exception>
#include <stdexcept>

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/Lifecycle.h>

//-- Network
#include <wsb-core/network/NetworkLayer.h>

//-- Broker
#include <wsb-core/broker/Broker.h>


#include "WSEngine.h"

namespace OSI {
namespace WSB {

WSEngine::WSEngine()  {


}

WSEngine::~WSEngine() {
	// TODO Auto-generated destructor stub
}

NetworkLayer& WSEngine::getNetwork() {

	return this->network;

}

Broker& WSEngine::getBroker() {

	return this->broker;

}

void WSEngine::init() {


    // UTF8 Support required
    //-----------------------------
   /* if (!setlocale(LC_CTYPE, "UTF-8")) {
        string msg = "Could not set the required UTF-8 Locale";
        Logging::getLogger("wsengine")->fatalStream() << msg.c_str();
        throw runtime_error(msg.c_str());
    }*/

	//-- Propagate Broker to network
	this->network.setBroker(&(this->broker));

	//-- Init Network
	this->network.init();

	//-- Init broker
	this->broker.init();
}

void WSEngine::start() {

	//-- Start Network layer
	this->network.start();

	//-- Start Broker
	this->broker.start();

}

void WSEngine::reload() {

	//-- Reload Broker
	this->broker.reload();

	//-- Reload network
	this->network.reload();

}

void WSEngine::stop() {

    Logging::getLogger("wsb.engine.finish")->infoStream() << "Stopping WSB Engine";

	//-- Stop Network layer
	this->network.stop();

	//-- Stop Broker
	this->broker.stop();
}

void WSEngine::finish() {

    Logging::getLogger("wsb.engine.finish")->infoStream() << "Finishing WSB Engine";

	//-- Finish Network layer
	this->network.finish();

	//-- Finish Broker
	this->broker.finish();
}


} /* namespace WSB */
} /* namespace OSI */
