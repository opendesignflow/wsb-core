/*
 * Dispatchable.h
 *
 *  Created on: Nov 16, 2011
 *      Author: rleys
 */

// Includes
//-------------------

//-- Dispatch
#include <wsb-core/common/dispatch/Runnable.h>

#ifndef DISPATCHABLE_H_
#define DISPATCHABLE_H_

namespace OSI {
namespace WSB {

//-- Dispatch
class DispatchQueue;


/**
 * A virtual class to implement a run() method that will be dispatched
 */
class Dispatchable : public Runnable {

protected:



public:
	Dispatchable();
	virtual ~Dispatchable();

	/**
	 * To be implemented.
	 * This contains the business method
	 */
	virtual void run() = 0;


	/** \defgroup Work Dispatch */
	/** @{ */

	/**
	 * Dispatch this dispatcheable on the target Queue and return
	 * @warning The Queue doesn't manage the deletion of the Dispatchable
	 * @param targetQueue
	 */
	void dispatch(DispatchQueue * targetQueue);

	/**
	 * Dispatch this dispatcheable on the target Queue
	 * and waits for it to be finished
	 * @warning The Queue doesn't manage the deletion of the Dispatchable
	 * @param targetQueue
	 */
	void dispatchAndWait(DispatchQueue * targetQueue);


	/**
	 * Waits on a semaphore for this task to be finished
	 */
	void waitFinished();

	/**
	 * Waits on a semaphore for this task to be finished
	 * Returns if not finished alread
	 * @return true if finished, false if not
	 */
	bool tryWaitFinished();


	/** @} */


	/** \defgroup Queue Interaction */
	/** @{ */

	/**
	 * This method is called by the Queue after successful dispatch.
	 * An Implementation of Dispatchable can block in here to synchronize the Queue on something.
	 * Useful to create a barrier for example
	 *
	 * Default: Don't do anything (don't block then)
	 */
	virtual void blockQueue();


	/** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* DISPATCHABLE_H_ */
