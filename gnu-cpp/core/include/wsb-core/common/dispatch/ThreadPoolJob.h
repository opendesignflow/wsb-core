/*
 * ThreadPoolJob.h
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

#ifndef THREADPOOLJOB_H_
#define THREADPOOLJOB_H_

// Includes
//-------------------



namespace OSI {
namespace WSB {

//-- Dispatch
class Runnable;

/**
 * Used by ThreadPool to have a reference to a submitted job and have some utilities
 */
class ThreadPoolJob {
protected:

	/// The base runnable
	Runnable * runnable;


public:
	ThreadPoolJob(Runnable *);
	virtual ~ThreadPoolJob();


	/**
	 * Call us isFinished on Runnable
	 * @return true if the runnable has finished, false otherwise
	 */
	bool isFinished();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* THREADPOOLJOB_H_ */
