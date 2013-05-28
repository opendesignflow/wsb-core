/*
 * ThreadPool.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- System
#include <unistd.h>

//-- Std
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>

//-- Dispatch
#include <wsb-core/common/dispatch/Thread.h>
#include <wsb-core/common/dispatch/ThreadPoolRunner.h>
#include <wsb-core/common/dispatch/ThreadPoolJob.h>
#include <wsb-core/common/dispatch/Runnable.h>

#include "ThreadPool.h"

namespace OSI {
namespace WSB {

ThreadPool::ThreadPool(int mainPoolSize) {

	//-- Defaults
	this->mainThreadPoolSize = mainPoolSize;

}

ThreadPool::~ThreadPool() {

	// Use finish to clear threads
	this->finish();

	// Clear remaining things (if some)
	// Nothing to do
}


void ThreadPool::init() {

	// Init Main Thread Pool
	//---------------------------------

	//-- If size is -1, try to estimate by getting the number of processors
	if (this->mainThreadPoolSize==-1) {
		this->mainThreadPoolSize = sysconf( _SC_NPROCESSORS_ONLN );
	}

	//-- Create
	for (int i = 0 ; i<this->mainThreadPoolSize ; i++) {

		//-- Create Threads
		stringstream name;
		name << "Runner " << i;
		ThreadPoolRunner * runner = new ThreadPoolRunner(name.str());
		this->mainThreadPool.push_back(runner);
	}

}


void ThreadPool::start() {

	//-- Start Threads
	for (vector<ThreadPoolRunner*>::iterator it = this->mainThreadPool.begin() ; it != this->mainThreadPool.end(); it++) {
		(*it)->start();
	}

}



void ThreadPool::stop() {

	//-- Stop Threads
	for (vector<ThreadPoolRunner*>::iterator it = this->mainThreadPool.begin() ; it != this->mainThreadPool.end(); it++) {

	   // Logging::getLogger("common.dispatch.threadpool.stop")->infoStream() << "Stopping Runner";

		(*it)->stop();
		(*it)->join();

		//Logging::getLogger("common.dispatch.threadpool.stop")->infoStream() << "Stopped Runner";
	}

}


void ThreadPool::finish() {

	//-- Stop Threads
	for (vector<ThreadPoolRunner*>::iterator it = this->mainThreadPool.begin() ; it != this->mainThreadPool.end(); it++) {
		(*it)->finish();
	}

	//-- Clean
	for (vector<ThreadPoolRunner*>::iterator it = this->mainThreadPool.begin() ; it != this->mainThreadPool.end(); it++) {
		delete (*it);
	}
	this->mainThreadPool.clear();

}

ThreadPoolJob * ThreadPool::trySubmit(Runnable * r) {

	//-- Try to submit to one free main Thread
	ThreadPoolJob * result = NULL;
	for (vector<ThreadPoolRunner*>::iterator it = this->mainThreadPool.begin() ; it != this->mainThreadPool.end(); it++) {

		//-- Submit if not busy
		if (!(*it)->isBusy()) {


			//-- Submit
			//cerr << "Submitted to Runner "<< (*it)->getName() << endl;
			(*it)->run(r);

			//-- Create a Job object to play with dispatched task
			result = new ThreadPoolJob(r);
			break;
		}
	}

	return result;

}

ThreadPoolJob * ThreadPool::submit(Runnable * r) {

	ThreadPoolJob * result = NULL;

	//-- Loop until one Thread gets free


	do {
		//-- Try to submit to one free main Thread
		for (vector<ThreadPoolRunner*>::iterator it = this->mainThreadPool.begin() ; it != this->mainThreadPool.end(); it++) {

			//-- Submit if not busy
			if (!(*it)->isBusy()) {

				//-- Submit
				(*it)->run(r);

				//-- Create a Job object to play with dispatched task
				result = new ThreadPoolJob(r);

				break;
			}
		}

		//-- If no more runners in vector(if finished for example) -> stop
		if (result==NULL && this->mainThreadPool.size()==0)
			break;

	} while (result==NULL);

	return result;

}


} /* namespace WSB */
} /* namespace OSI */
