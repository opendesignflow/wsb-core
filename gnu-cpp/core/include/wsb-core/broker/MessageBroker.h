/*
 * MessageBroker.h
 *
 *  Created on: Nov 16, 2011
 *      Author: rleys
 */

#ifndef MESSAGEBROKER_H_
#define MESSAGEBROKER_H_

// Includes
//-----------------

//-- Dispatch
#include <wsb-core/common/dispatch/Dispatchable.h>
#include <wsb-core/common/dispatch/ParallelDispatchQueue.h>
#include <wsb-core/common/dispatch/SerialDispatchQueue.h>

//-- Broker
#include <wsb-core/broker/Broker.h>

namespace OSI {
namespace WSB {

//-- Engine
class WSEngine;

//-- Message
class MessageContext;

//-- Broker
class BrokeringTree;

class MessageBroker : public Dispatchable {

protected:

	/// The Network Context: Necessary to forward message to network layer
	NetworkLayer * networkLayer;

	/// The message to broker
	MessageContext * 		messageContext;

	/// The brokering tree to apply
	/// This is a copy To isolate brokering
	BrokeringTree  		brokeringTree;




public:
	MessageBroker(NetworkLayer *,MessageContext *,BrokeringTree );
	virtual ~MessageBroker();

	/**
	 * Dispatch Method
	 */
	virtual void run();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGEBROKER_H_ */
