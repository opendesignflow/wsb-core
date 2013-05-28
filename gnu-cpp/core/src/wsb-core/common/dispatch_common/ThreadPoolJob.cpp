/*
 * ThreadPoolJob.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- Dispatch
#include <wsb-core/common/dispatch/Runnable.h>

#include "ThreadPoolJob.h"

namespace OSI {
namespace WSB {

ThreadPoolJob::ThreadPoolJob(Runnable * runnable) {
	this->runnable = runnable;

}

ThreadPoolJob::~ThreadPoolJob() {

}


bool ThreadPoolJob::isFinished() {
	return this->runnable->isFinished();
}

} /* namespace WSB */
} /* namespace OSI */
