/*
 * ThreadPool.h
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

// Includes
//--------------

//-- Std
#include <vector>
using namespace std;



namespace OSI {
namespace WSB {


//-- Dispatch
class Runnable;
class ThreadPoolRunner;
class ThreadPoolJob;

/**
 * A thread pool executes runnable Objects, using a set a threads that are maintained alive, or some reserve threads that are
 * allowed to be created to face a surge of workload
 */
class ThreadPool {

protected:

	/// Size of the main Thread pool
	int							mainThreadPoolSize;

	/// Holds the main Runners
	vector<ThreadPoolRunner*> 	mainThreadPool;



public:

	/**
	 * Please use lifecycle init to initialise the ThreadPool
	 * @param mainPoolSize if -1, pool size is automatically defined
	 */
	ThreadPool(int mainPoolSize = -1);
	virtual ~ThreadPool();


	/** \defgroup Lifecycle */
	/**@{*/

	/**
	 * Creates the runners objects
	 */
	void init();

	/**
	 * Starts all runners
	 */
	void start();

	/**
	 * Wait for jobs to be finished and stops runners
	 * This method does not return until all Running jobs have finished
	 */
	void stop();

	/**
	 * Wait for jobs to be finished and stops runners
	 * Cleans the Runners objects
	 */
	void finish();

	/**}@*/

	/** \defgroup Job submit */
	/** @{ */

	/**
	 * Submits the runnable to a thread.
	 * @param a runnable to be run
	 * @return a job object if the job was submitted, NULL if not already
	 * @warning The returned Job is to be deleted by user
	 */
	ThreadPoolJob * trySubmit(Runnable *);

	/**
	 * Submits the runnable to a thread.
	 * This method waits and blocks until a thread is available to submit the runnable
	 * @param a runnable to be run
	 * @return a job object if the job was submitted, NULL if there has been an error
	 * @warning The returned Job is to be deleted by user
	 */
	ThreadPoolJob * submit(Runnable *);

	/** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* THREADPOOL_H_ */
