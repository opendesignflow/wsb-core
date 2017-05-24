/*
 * ThreadPoolRunner.h
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

#ifndef THREADPOOLRUNNER_H_
#define THREADPOOLRUNNER_H_

// Includes
//---------------

//-- Dispatch
#include <wsb-core/common/dispatch/Thread.h>

//-- Sync
#include <pthread.h>
#include <semaphore.h>

namespace OSI {
namespace WSB {

//-- Dispatch
class Runnable;

/**
 * This class is a Thread that waits for a Runnable to be offered and runs it.
 * When the run() method of the Runnable if finished, it starts waiting for another Runnable to be submitted
 *
 * DOCU Explain synchronisation mechanism
 *
 */
class ThreadPoolRunner : public Thread {

protected:

	/// The actual runnable to run
	/// NULL if none
	Runnable * 		runnable;

	/// Mutex to protect checks on runnable
	pthread_mutex_t runnableMutex;

	/// Semaphore to give Runnable
	sem_t 			runSemaphore;




public:
	ThreadPoolRunner(string name = "");
	virtual ~ThreadPoolRunner();

	/**
	 * Implements the wait for runnable logic
	 */
	virtual void run();


	/** \defgroup Lifecycle Lifecycle management */
	/** @{ */

	/**
	 * This stop method releases a grant in the runnable wait semaphore if not already busy,
	 * Otherwise the thread will endlessly wait preventing stopping
	 */
	virtual void stop();

	/** @} */

	/** \defgroup Runnable submission */
	/**@{*/

	/**
	 * Submits the provided runnable to be run
	 * @warning This method blocks. The provided Runnable has to be memory managed by user
	 * @param
	 */
	void run(Runnable *);

	/**
	 *
	 * @return true if the thread is running a Runnable already, false if not
	 */
	bool isBusy();

	/**}@*/

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* THREADPOOLRUNNER_H_ */
