/*
 * ParallelDispatchQueue.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std
#include <iostream>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>

//-- Dispatch
#include <wsb-core/common/dispatch/Dispatchable.h>
#include <wsb-core/common/dispatch/DispatchQueue.h>
#include <wsb-core/common/dispatch/ThreadPoolJob.h>
#include <wsb-core/common/dispatch/BarrierDispatchable.h>

#include "ParallelDispatchQueue.h"

namespace OSI {
namespace WSB {

ParallelDispatchQueue::ParallelDispatchQueue() {

	//-- Defaults
	this->dispatchedJobsCleanFrequency = 10;

	//-- Init Thread Pool
	this->dispatchPool.init();

}

ParallelDispatchQueue::~ParallelDispatchQueue() {
	// TODO Auto-generated destructor stub
}


void ParallelDispatchQueue::run() {

	//-- Start the Thread Pool
	this->dispatchPool.start();

	//-- Control data
	unsigned int loopCount = 0; // (for cleaning the jobs list)

	//-- Sync on stop
	while (!this->stopRequested()) {

		//-- Get a Dispatchable (blocks)
		Dispatchable * dispatchable = this->takeFromQueue();

		//-- If NULL, stop
		if (dispatchable==NULL)
			continue;

		// Normal Dispatchable
		//--------------------------

		//-- Try to submit to thread pool
		ThreadPoolJob * job = this->dispatchPool.submit(dispatchable);
		if (job!=NULL) {

			//-- Store in current Jobs list
			this->dispatchedJobs.push_back(job);

			//-- Let Dispatchable block queue if needed
			dispatchable->blockQueue();
		}

		//-- Else loop and retry: Maybe clean jobs list
		if (loopCount++>this->dispatchedJobsCleanFrequency) {

			//-- Clean jobs list
			loopCount = 0;
			this->getRemainingJobs(); // (this method cleans)

			//-- If we are down, just wait

		}

	}

	//-- Stop Thread pool

	Logging::getLogger("common.dispatch.parallelqueue.stop")->warnStream() << "Stopping Parallel Dispatch Queue";

	//this->barrier()->waitFinished();
	this->dispatchPool.stop();
	int remaining = this->getRemainingJobs();

	Logging::getLogger("common.dispatch.parallelqueue.stop")->warnStream() << "Stopped Parallel Dispatch Queue has "<< remaining <<" remaining unfinished jobs";



}


Dispatchable * ParallelDispatchQueue::barrier() {

	//-- Create Barrier
	BarrierDispatchable * barrier = new BarrierDispatchable(this);
	this->dispatch(barrier);


	return barrier;
}


int ParallelDispatchQueue::getRemainingJobs() {

	//-- Go through list, and count unfinished jobs.
	//-- Remove the finished one from list and delete
	int count = 0;
	list<ThreadPoolJob*> toDelete;
	for (list<ThreadPoolJob*>::iterator it = this->dispatchedJobs.begin();it!=this->dispatchedJobs.end();it++) {
		if ((*it)->isFinished()) {

			//-- Mark to remove
			toDelete.push_front((*it));
			//this->dispatchedJobs.remove((*it));
			//delete (*it);

		} else
			count++;
	}

	//-- Clean
	for (list<ThreadPoolJob*>::iterator it = toDelete.begin();it!=toDelete.end();it++) {

		this->dispatchedJobs.remove((*it));
		delete (*it);

	}



	return count;

}

} /* namespace WSB */
} /* namespace OSI */
