/*
 * ParallelDispatchQueue.h
 *
 *  Created on: Nov 17, 2011
 *      Author: rleys
 */

#ifndef PARALLELDISPATCHQUEUE_H_
#define PARALLELDISPATCHQUEUE_H_

// Includes
//----------------

//-- Std
#include <list>
using namespace std;

//-- Dispatch
#include <wsb-core/common/dispatch/DispatchQueue.h>
#include <wsb-core/common/dispatch/ThreadPool.h>

namespace OSI {
namespace WSB {

//-- Dispatch
class Dispatchable;

/**
 * This class can be configured to have a variable size of thread pool
 * The default Threadpool follows specification of Thread pool
 *
 * FIXME: getRemainingJobs mabe wrongly deleting
 *
 */
class ParallelDispatchQueue: public DispatchQueue {

    protected:

        /// A Thread pool to dispatch threads
        ThreadPool dispatchPool;

        /// A list to maintain the list of thread pool jobs that have been submitted
        list<ThreadPoolJob*> dispatchedJobs;

        /// The number of dispatch thread loops to wait before cleaning the dispatchedJobs list
        /// Default: 10
        unsigned int dispatchedJobsCleanFrequency;

    public:
        ParallelDispatchQueue();
        virtual ~ParallelDispatchQueue();

        /** \defgroup WorkDispatch Work Dispatch */
        /** @{ */

        /**
         * Dispatch a barrier task
         * This barrier task will only be finished when no other tasks are available in the queue
         * @return
         */
        Dispatchable * barrier();

        /**
         * Thread Method to do the job of dispatching tasks when some are finished.
         */
        void run();

        /** @} */

        /**\defgroup JobsManagement Jobs management */
        /**@{*/

        /**
         *
         * @return The unmber of jobs that are running
         */
        int getRemainingJobs();

        /**}@*/
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* PARALLELDISPATCHQUEUE_H_ */
