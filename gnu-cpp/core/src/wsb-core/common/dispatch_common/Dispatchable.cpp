/*
 * Dispatchable.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: rleys
 */


// Includes
//-----------------------


//-- Dispatch
#include <wsb-core/common/dispatch/DispatchQueue.h>

#include "Dispatchable.h"


namespace OSI {
namespace WSB {

extern "C"
{
    // this C function will be used to receive the thread and pass it back to the Thread instance
    void dispatch_catch(void* arg) {
        Dispatchable* t = static_cast<Dispatchable*>(arg);
        t->run();
    }
}

Dispatchable::Dispatchable() {
	// TODO Auto-generated constructor stub

}

Dispatchable::~Dispatchable() {
	// TODO Auto-generated destructor stub
}


void Dispatchable::dispatch(DispatchQueue * targetQueue) {
	targetQueue->dispatch(this);
}

void Dispatchable::dispatchAndWait(DispatchQueue * targetQueue) {
	targetQueue->dispatchAndWait(this);
}

void Dispatchable::waitFinished() {

	//while(!this->isFinished()) pthread_yield();
	Runnable::waitFinished();
}

bool Dispatchable::tryWaitFinished() {
	return false;
}


void Dispatchable::blockQueue() {

}

} /* namespace WSB */
} /* namespace OSI */
