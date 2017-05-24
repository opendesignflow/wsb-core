/*
 * MutexLocker.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Sync
#include <pthread.h>

#include "MutexLocker.h"

namespace OSI {
namespace WSB {

MutexLocker::MutexLocker(pthread_mutex_t * lock) {

	//-- Register and lock
	pthread_mutex_lock(lock);
	this->lock = lock;

}

MutexLocker::~MutexLocker() {

	//-- Release lock
	pthread_mutex_unlock(this->lock);
}

} /* namespace WSB */
} /* namespace OSI */
