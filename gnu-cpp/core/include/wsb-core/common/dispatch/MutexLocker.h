/*
 * MutexLocker.h
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

#ifndef MUTEXLOCKER_H_
#define MUTEXLOCKER_H_

// Includes
//----------------

//-- Sync
#include <pthread.h>

namespace OSI {
namespace WSB {

/**
 * Mutex Locker takes a reference to a pthread mutex, an locks it in constructor.
 * When released (destroyed), it unlocks the mutex
 */
class MutexLocker {

private:

	pthread_mutex_t * lock;

public:
	MutexLocker(pthread_mutex_t *);
	virtual ~MutexLocker();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MUTEXLOCKER_H_ */
