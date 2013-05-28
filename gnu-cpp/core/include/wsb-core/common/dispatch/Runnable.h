/*
 * Runnable.h
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

// Includes
//------------

//-- Dispatch
#include <wsb-core/common/dispatch/MutexLocker.h>

//-- Pthread
#include <pthread.h>


namespace OSI {
namespace WSB {

/**
 * An object that can be runned in a thread.
 * The run() method should contain the business code
 *
 * Some more utility are provided to know when a job will be finished or so
 *
 */
class Runnable {

protected:

	/// Mutex to protected finished boolean
	pthread_mutex_t finishedMutex;


	/// Boolean marker to know if the Runnable is finished
	pthread_cond_t	finishedCondition;

	bool 			finished;

public:
	Runnable();
	virtual ~Runnable();

	/**
	 * To be overriden with business code
	 */
	virtual void run() = 0;

	/**\defgroup Utilities */
	/**@{*/


	/// Indicate the runnable completed
	void setFinished();

	/**
	 * @see #setFinished
	 * @return true if the runnable has finished, false either
	 */
	bool isFinished();

	/**
	 * Waits for this Runnable to be finished
	 * @return
	 */
	void waitFinished();

	/**}@*/

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* RUNNABLE_H_ */
