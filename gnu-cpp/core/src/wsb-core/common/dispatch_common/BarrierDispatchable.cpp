/*
 * BarrierDispatchable.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- semaphore
#include <semaphore.h>

//-- Dispatch
#include <wsb-core/common/dispatch/ParallelDispatchQueue.h>

#include "BarrierDispatchable.h"

namespace OSI {
namespace WSB {

BarrierDispatchable::BarrierDispatchable(ParallelDispatchQueue * queue) {

	//-- Init Sempaphore
	sem_init(&(this->blockQueueSempahore),0,0);

	//-- Target queue
	this->queue = queue;


}

BarrierDispatchable::~BarrierDispatchable() {

	//-- Destroy sempahore
	sem_destroy(&(this->blockQueueSempahore));

}

void BarrierDispatchable::run() {

	//-- Wait for jobs to be only one remaining (this one)
	while (this->queue->getRemainingJobs()>1) pthread_yield();

	//-- Post into sempahore
	sem_post(&(this->blockQueueSempahore));

}

void BarrierDispatchable::blockQueue() {

	/// Wait on sempaphore
	sem_wait(&(this->blockQueueSempahore));
}

} /* namespace WSB */
} /* namespace OSI */
