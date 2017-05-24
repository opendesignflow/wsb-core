/*
 * SerialDispatchQueue.h
 *
 *  Created on: Nov 17, 2011
 *      Author: rleys
 */

#ifndef SERIALDISPATCHQUEUE_H_
#define SERIALDISPATCHQUEUE_H_

// Includes
//----------------

//-- Dispatch
#include <wsb-core/common/dispatch/DispatchQueue.h>

namespace OSI {
namespace WSB {

class SerialDispatchQueue : public DispatchQueue {
public:
	SerialDispatchQueue();
	virtual ~SerialDispatchQueue();

	/**
	 * Does the job of dispatching the Tasks in the queue
	 */
	virtual void run();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* SERIALDISPATCHQUEUE_H_ */
