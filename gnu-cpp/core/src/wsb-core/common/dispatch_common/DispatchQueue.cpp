/*
 * DispatchQueue.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Dispatch
#include <wsb-core/common/dispatch/Dispatchable.h>
#include <wsb-core/common/dispatch/SafeBlockingQueue.h>

#include "DispatchQueue.h"

namespace OSI {
namespace WSB {

DispatchQueue::DispatchQueue() {

	// Init workqueue Mutex
	this->workQueueMutex = PTHREAD_MUTEX_INITIALIZER;
	this->workqueue = NULL;
}

DispatchQueue::~DispatchQueue() {

	//-- Destroy Dispatch Queue
	delete this->workqueue;
}

void DispatchQueue::start() {

	//-- Create Dispatch Queue
	if (this->workqueue==NULL)
		this->workqueue = new SafeBlockingQueue<Dispatchable*>();

	//-- Start thread
	Thread::start();

}

void DispatchQueue::stop() {

	//-- Destroy Dispatch Queue
	delete this->workqueue;
	this->workqueue = NULL;

	//-- Stop thread
	Thread::stop();
}



void DispatchQueue::lockQueue() {
	pthread_mutex_lock(&(this->workQueueMutex));
}

void DispatchQueue::unlockQueue() {
	pthread_mutex_unlock(&(this->workQueueMutex));
}

void DispatchQueue::offerToQueue(Dispatchable * dispatchable) {

	//-- Lock
	/*this->lockQueue();

	//-- Offer
	this->workqueue.push(dispatchable);

	//-- Unlock
	this->unlockQueue();*/
	this->workqueue->offer(dispatchable);
}

Dispatchable * DispatchQueue::takeFromQueue() {


	return this->workqueue->getBlocking(1000);

}




void DispatchQueue::dispatch(Dispatchable * dispatchable) {

	//-- Just offer to queue
	this->offerToQueue(dispatchable);

}

void DispatchQueue::dispatchAndWait(Dispatchable * dispatchable) {

	//-- Just offer to queue
	this->offerToQueue(dispatchable);

	//-- Wait on it
	dispatchable->waitFinished();

}

} /* namespace WSB */
} /* namespace OSI */
