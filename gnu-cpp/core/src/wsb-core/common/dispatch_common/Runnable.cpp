/*
 * Runnable.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- Pthread
#include <pthread.h>

//-- Std
#include <iostream>
using namespace std;

#include "Runnable.h"

namespace OSI {
namespace WSB {

Runnable::Runnable() {

	this->finishedMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&(this->finishedCondition),NULL);
	this->finished = false;
}

Runnable::~Runnable() {
	//-- Destroy Condition
	pthread_cond_destroy(&(this->finishedCondition));
	pthread_mutex_destroy(&(this->finishedMutex));
}



void Runnable::setFinished() {

	//cerr << "Trying to Locked finished mutex to set finished "  << endl;
	pthread_mutex_lock(&(this->finishedMutex));
	//cerr << "Locked finished mutex to set finished "  << endl;

	pthread_cond_broadcast(&(this->finishedCondition));
	this->finished = true;

	pthread_mutex_unlock(&(this->finishedMutex));
}


bool Runnable::isFinished() {

	pthread_mutex_lock(&(this->finishedMutex));

	bool result = this->finished;

	pthread_mutex_unlock(&(this->finishedMutex));

	return result;
}

void Runnable::waitFinished() {

	pthread_mutex_lock(&(this->finishedMutex));
	pthread_cond_wait(&(this->finishedCondition),&(this->finishedMutex));
	pthread_mutex_unlock(&(this->finishedMutex));
}

} /* namespace WSB */
} /* namespace OSI */
