/*
 * DispatchQueue.h
 *
 *  Created on: Nov 17, 2011
 *      Author: rleys
 */

#ifndef DISPATCHQUEUE_H_
#define DISPATCHQUEUE_H_

// Includes
//-----------------

//-- Std
#include <queue>
using namespace std;

//-- Pthread
#include <pthread.h>

//-- Dispatch
#include <wsb-core/common/dispatch/Thread.h>

namespace OSI {
namespace WSB {

//-- Dispatch
class Dispatchable;
template<class T> class SafeBlockingQueue;

/**
 * A Queue that executes tasks submitted to it.
 * It can be either serial of parallel.
 *
 * <h1>Serial Queue</h1>
 *
 * All tasks are executed one after the other
 *
 * <h1>Parallel Queue</h1>
 *
 * Tasks may be executed in parallel, using a Thread pool that can be configured
 *
 * Each task is running a thread that takes care of dispatching
 *
 * DOCU Explain synchronisation for queue offering
 *
 */
class DispatchQueue: public Thread {

    protected:

        /// The queue of Dispatchable tasks
        SafeBlockingQueue<Dispatchable*> * workqueue;

        /// Mutex for workqueue
        pthread_mutex_t workQueueMutex;

    public:
        DispatchQueue();
        virtual ~DispatchQueue();

        /**
         * Does the job of dispatching the Tasks in the queue
         */
        virtual void run() = 0;

        /** \defgroup Lifecycle */
        /** @{ */

        /**
         * Creates the workqueue
         */
        virtual void start();

        /**
         * Deletes the workqueue to release thread
         */
        virtual void stop();

        /** @} */

        /** \defgroup Workqueue manipulation */
        /** @{ */

        /**
         * Offer the provided Dispatchable to queue
         * This method relies on implemented lock/unlockQueue operations
         * @param
         */
        void offerToQueue(Dispatchable *);

        /**
         * Gets a Dispatchable from the Queue
         * This method should be blocking, and returning NULL on stop
         * It is non blocking to allow dispatch thread to synchronise on stopping
         * @warning The returned Dispatchable is removed from work queue
         * @return  A Dispatchable or NULL if none or an error occured (like queue destroyed)
         */
        Dispatchable * takeFromQueue();

    protected:

        /// Release the lock on the Queue
        virtual void unlockQueue();

        /// Take a lock on the Queue
        virtual void lockQueue();

        /** @} */

        /** \defgroup Work Dispatch */
        /** @{ */

    public:

        /**
         * Register the provided task for dispatch and return
         * Simply offers the work item to the queue
         * @warning The Queue doesn't manage the deletion of the Dispatchable
         * @param
         */
        virtual void dispatch(Dispatchable *);

        /**
         * Register the provided task for dispatch, and waits for it to have been executed
         * @warning The Queue doesn't manage the deletion of the Dispatchable
         * @param
         */
        virtual void dispatchAndWait(Dispatchable*);

        /** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* DISPATCHQUEUE_H_ */
