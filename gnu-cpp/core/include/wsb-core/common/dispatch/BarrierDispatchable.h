/*
 * BarrierDispatchable.h
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

#ifndef BARRIERDISPATCHABLE_H_
#define BARRIERDISPATCHABLE_H_

// Include
//------------------

//-- pthread
#include <pthread.h>
#include <semaphore.h>


//-- Dispatch
#include <wsb-core/common/dispatch/Dispatchable.h>


namespace OSI {
namespace WSB {

//-- Dispatch
class ParallelDispatchQueue;

/**
 * If submitted to a queue, blocks the Queue until all the previously dispatched jobs have completed
 */
class BarrierDispatchable: public Dispatchable {

protected:

	/// Sempahore to block the queue
	sem_t	blockQueueSempahore;

	/// The queue to check up on
	ParallelDispatchQueue * queue;

public:
	BarrierDispatchable(ParallelDispatchQueue *);
	virtual ~BarrierDispatchable();

	/// The run method checks the Queue Planned jobs are all finished
	virtual void run();

	virtual void blockQueue();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* BARRIERDISPATCHABLE_H_ */
