/*
 * DispatchIntermediaryUp.h
 *
 *  Created on: Nov 22, 2011
 *      Author: rleys
 */

#ifndef DISPATCHINTERMEDIARYUP_H_
#define DISPATCHINTERMEDIARYUP_H_

// Includes
//-----------

//-- Dispatch
#include <wsb-core/common/dispatch/Dispatchable.h>


namespace OSI {
namespace WSB {

//-- Broker
class Intermediary;

//-- Message
class MessageContext;


class DispatchIntermediaryUp: public Dispatchable {

protected:

	/// The intermediary to down
	Intermediary * intermediary;

	/// The message context
	MessageContext * context;

public:
	DispatchIntermediaryUp(Intermediary * ,MessageContext *);;
	virtual ~DispatchIntermediaryUp();

	virtual void run();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* DISPATCHINTERMEDIARYUP_H_ */
