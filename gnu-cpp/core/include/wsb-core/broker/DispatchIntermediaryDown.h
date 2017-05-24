/*
 * DispatchIntermediaryDown.h
 *
 *  Created on: Nov 22, 2011
 *      Author: rleys
 */

#ifndef DISPATCHINTERMEDIARYDOWN_H_
#define DISPATCHINTERMEDIARYDOWN_H_

// Includes
//--------------------

//-- Dispatch
#include <wsb-core/common/dispatch/Dispatchable.h>

namespace OSI {
namespace WSB {

//-- Broker
class Intermediary;

//-- Message
class MessageContext;

/**
 * This classes runs the down method on provided intermediary
 */
class DispatchIntermediaryDown: public Dispatchable {

protected:

	/// The intermediary to down
	Intermediary * intermediary;

	/// The message context
	MessageContext * context;

public:
	DispatchIntermediaryDown(Intermediary * ,MessageContext *);
	virtual ~DispatchIntermediaryDown();


	virtual void run();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* DISPATCHINTERMEDIARYDOWN_H_ */
