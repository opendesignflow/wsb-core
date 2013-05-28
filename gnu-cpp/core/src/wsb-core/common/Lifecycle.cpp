/*
 * Lifecycle.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

// Includes
//-------------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/LifecycleException.h>

#include "Lifecycle.h"

namespace OSI {
namespace WSB {

Lifecycle::Lifecycle() {

	//-- Init state
	this->lifecycleState = LIFECYCLE_INITIAL;

}

Lifecycle::~Lifecycle() {
	// TODO Auto-generated destructor stub
}

void Lifecycle::init() {
    this->lifecycleProgressToStopped();
}

void Lifecycle::start(){
    this->lifecycleProgressToStarted();
}

void Lifecycle::reload(){

}

void Lifecycle::stop(){
    this->lifecycleProgressToStopped();
}

void Lifecycle::finish() {

}

LIFECYCLE_STATE Lifecycle::getLifecycleState() {
    return this->lifecycleState;
}


void Lifecycle::lifecycleProgressToStopped() throw (LifecycleException) {

	// FIXME Check conditions
	this->lifecycleState = LIFECYCLE_STOPPED;
}
void Lifecycle::lifecycleProgressToStarted() throw (LifecycleException) {

	// FIXME conditions
	this->lifecycleState = LIFECYCLE_STARTED;
}


void Lifecycle::lifecycleAssert(LIFECYCLE_STATE givenState,string caller) throw (LifecycleException) {

	if (this->lifecycleState != givenState) {
		LifecycleException ex;
		ex  << "Asserting in state " << givenState << " but in " << (this->lifecycleState) << "in " << caller;
		throw ex;
	}

}

void Lifecycle::lifecycleAssertNot(LIFECYCLE_STATE givenState) throw (LifecycleException) {

	if (this->lifecycleState == givenState) {
		LifecycleException ex;
		ex  << "Asserting not in state " << givenState << " but in " << this->lifecycleState;
		throw ex;
	}

}

} /* namespace WSB */
} /* namespace OSI */
