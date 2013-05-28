/*
 * DispatchIntermediaryUp.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Intermediary
#include <wsb-core/broker/Intermediary.h>

#include "DispatchIntermediaryUp.h"

namespace OSI {
namespace WSB {

DispatchIntermediaryUp::DispatchIntermediaryUp(Intermediary * intermediary ,MessageContext * context) {

	//-- Init
	this->intermediary = intermediary;
	this->context = context;

}

DispatchIntermediaryUp::~DispatchIntermediaryUp() {
	// TODO Auto-generated destructor stub
}

void DispatchIntermediaryUp::run() {

}

} /* namespace WSB */
} /* namespace OSI */
