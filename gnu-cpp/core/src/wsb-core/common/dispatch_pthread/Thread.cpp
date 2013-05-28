/*
 * Thread.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */


// Includes
//----------------------

//-- Std
#include <errno.h>
using namespace std;

//-- Sync
#include <pthread.h>
#include <semaphore.h>

//-- Dispatch
#include <wsb-core/common/dispatch/MutexLocker.h>

#include "Thread.h"

namespace OSI {
namespace WSB {


extern "C"
{
    // this C function will be used to receive the thread and pass it back to the Thread instance
    void* thread_catch(void* arg) {
        Thread* t = static_cast<Thread*>(arg);
        t->run();
        return 0;
    }
}

Thread::Thread(string name) {

	//-- Thread name
	this->threadName = name;

	//-- Init sync points
	sem_init(&(this->sync_stop), 0, 0);
	this->sync_stop_mutex = PTHREAD_MUTEX_INITIALIZER;

	sem_init(&(this->sync_started), 0, 0);
    this->sync_started_mutex = PTHREAD_MUTEX_INITIALIZER;

}

Thread::~Thread() {


}

string Thread::getName() {
	return this->threadName;
}

void Thread::start() {

	//-- Create Thread
	int iret1 = pthread_create( &(this->threadHandle), NULL, thread_catch, this);


}

void Thread::stop() {

	//-- Increment stop sync point
	MutexLocker semLock(&(this->sync_stop_mutex));
	sem_post(&(this->sync_stop));


}


void Thread::finish() {

	//pthread_cancel(this->threadHandle);

	sem_close((&(this->sync_stop)));
    sem_close((&(this->sync_started)));
}


void Thread::join() {

	void * returnValue;
	pthread_join(this->threadHandle,&returnValue);

}


void Thread::joinStarted() {

    //MutexLocker semLock(&(this->sync_stop_mutex));
    sem_wait(&(this->sync_started));


}

bool Thread::stopRequested() {

	MutexLocker semLock(&(this->sync_stop_mutex));
	return sem_trywait(&(this->sync_stop))==-1 && errno == EAGAIN ? false : true;

}

void Thread::signalStarted() {

    sem_post(&(this->sync_started));

}


} /* namespace WSB */
} /* namespace OSI */
