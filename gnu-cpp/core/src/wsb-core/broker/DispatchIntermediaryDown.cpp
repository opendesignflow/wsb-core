/*
 * DispatchIntermediaryDown.cpp
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

#include "DispatchIntermediaryDown.h"

namespace OSI {
namespace WSB {

DispatchIntermediaryDown::DispatchIntermediaryDown(Intermediary * intermediary ,MessageContext * context){

	//-- Init
	this->intermediary = intermediary;
	this->context = context;

}

DispatchIntermediaryDown::~DispatchIntermediaryDown() {
	// TODO Auto-generated destructor stub
}

void DispatchIntermediaryDown::run() {

}

} /* namespace WSB */
} /* namespace OSI */
