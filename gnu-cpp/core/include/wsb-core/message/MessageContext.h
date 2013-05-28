/*
 * MessageContext.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef MESSAGECONTEXT_H_
#define MESSAGECONTEXT_H_

// Includes
//----------------

//-- Std
#include <string>
using namespace std;

//-- Broker
#include <wsb-core/broker/IntermediaryException.h>

//-- Message
#include <wsb-core/message/Message.h>

//-- Network
#include <wsb-core/network/NetworkContext.h>


namespace OSI {
namespace WSB {

/**
 * The MessageContext holds all information necessary to process a message,
 * from its reception by a connector to its response generation
 */
class MessageContext {

public:

	/// Direction for brokering
	enum DIRECTION {
		DOWN,
		UP
	};

private:

	/// The direction of the message
	DIRECTION	 		messageDirection;

	/// The Message, Mandatory
	Message * 			message;

	/// The Network context
	NetworkContext *	networkContext;

	/// The Error associated with this message
	IntermediaryException * error;

	/// The possible response to this message
	MessageContext * responseContext;

public:
	MessageContext(Message * message);
	MessageContext(Message * message,NetworkContext * networkContext);
	virtual ~MessageContext();

	/** \defgroup Content Content Management */
	/** @{*/

	/**
	 * Returns the message
	 * @return
	 */
	Message * 	getMessage();

	/**
	 * Returns the network context
	 * @return NULL if none registered
	 */
	NetworkContext *	getNetworkContext();

	/**
	 * Returns the error, or NULL if none registered
	 * @return
	 */
	IntermediaryException * getError();

	/**
	 * Sets the error for this message context.
	 *
	 * @param exception Provided exception is copied then managed by this object
	 */
	void setError(IntermediaryException exception);


	/**}@*/

	/**\defgroup MessageManipulation Message Manipulation */
	/**@{*/

	/**
	 * Causes the message to be reversed direction
	 * This means for example, that the content is going to be reseted, but the actual state saved
	 */
	void loop();

	/// Set the message to be upped
	void up();


	/// Set the message to be downed
	void down();

	/// Returns the message direction
	DIRECTION getDirection();


	void setDirection(DIRECTION dir);

	/**}@*/


	/**\defgroup Response Response Utilities */
    /**@{*/

	MessageContext * getResponseContext();

	void setResponseContext(MessageContext *);


	/**}@*/

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGECONTEXT_H_ */
