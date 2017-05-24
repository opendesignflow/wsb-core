/*
 * ThreadPoolRunner.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

// Includes
//--------------

//-- Std
#include <cerrno>
using namespace std;

//-- Std
#include <iostream>
using namespace std;

//-- Semaphore
#include <semaphore.h>

//-- Dispatch
#include <wsb-core/common/dispatch/MutexLocker.h>
#include <wsb-core/common/dispatch/Runnable.h>

#include "ThreadPoolRunner.h"

namespace OSI {
namespace WSB {

ThreadPoolRunner::ThreadPoolRunner(string name) : Thread(name) {

	//-- Defaults
	sem_init(&(this->runSemaphore),0,0);
	this->runnable = NULL;
	this->runnableMutex = PTHREAD_MUTEX_INITIALIZER;
}

ThreadPoolRunner::~ThreadPoolRunner() {
	//-- Destroy semaphore and mutex
	sem_destroy(&(this->runSemaphore));
	pthread_mutex_destroy(&(this->runnableMutex));
}


void ThreadPoolRunner::run() {

	//-- Sync on stop
	while (!this->stopRequested()) {

		//-- Wait on Semaphore to get a new runnable
		sem_wait(&(this->runSemaphore));

		//-- Lock run mutex
		//MutexLocker runlocker(&(this->runMutex));



		//-- Is there a Runnable to be run ?
		if (this->runnable!=NULL) {

			//-- Run
			//cerr << "Running Job" << endl;
			this->runnable->run();

			//-- mark runnable as finished
			this->runnable->setFinished();

			//-- Forget about the runnable
			MutexLocker runnableLock(&(this->runnableMutex));
			this->runnable = NULL;

		}

		//-- Unlock Performed by mutex locker (provides Exception safety here)
		//delete &(runlocker);
	}


}

bool ThreadPoolRunner::isBusy() {


	bool result = false;


	//-- Try to see if there is a credit available in the semaphore
	if (sem_trywait(&(this->runSemaphore))!=0) {

		//-- Could not lock semaphore -> No credit
		//-- If no runnable -> not busy, otherwise busy
		MutexLocker runnableLock(&(this->runnableMutex));
		if (this->runnable==NULL)
			result = false;
		else
			result = true;

	} else {

		//-- Could lock semaphore, check value of runnable to ensure we didn't steal
		//-- The run() credit
		MutexLocker runnableLock(&(this->runnableMutex));
		if (this->runnable!=NULL) {

			//-- Credit stolen, busy and regive the credit
			sem_post(&(this->runSemaphore));
			result = true;
		} else {
			//-- We could lock, but no runnable, should not happen anyway
		}

		//-- If we


	}

	//-- If the runMutex is locked, then it shall be busy
	return result;
}

void ThreadPoolRunner::run(Runnable * runnable) {

	//-- Submit
	MutexLocker runnableLock(&(this->runnableMutex));
	this->runnable = runnable;

	//-- Deliver Semaphore
	sem_post(&(this->runSemaphore));



}
 void ThreadPoolRunner::stop() {
    Thread::stop();

    if (!this->isBusy()) {
        sem_post(&(this->runSemaphore));
    }

}



} /* namespace WSB */
} /* namespace OSI */
