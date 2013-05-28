/*
 * Thread.h
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

#ifndef THREAD_H_
#define THREAD_H_

// Includes
//----------------------

//-- Std
#include <string>
using namespace std;

//-- Sync
#include <pthread.h>
#include <semaphore.h>

namespace OSI {
namespace WSB {

/**
 *
 * Defines default methods to be implemented for classes that should run as Threads
 *
 * Implemented ourselves to limit the number of control interfaces to what we really only need
 *
 */
class Thread {

    protected:

        /// The Thread Handle
        pthread_t threadHandle;

        /// Thread name
        string threadName;

        /** \defgroup Synchonisation points */
        /**@{*/

        /// Synchonisation point for implementation to indicate a started state
        sem_t sync_started;

        /// Synchonisation point to started thread Semaphore protection mutex
        pthread_mutex_t sync_started_mutex;

        /// Synchonisation point to stop thread (with stop)
        sem_t sync_stop;

        /// Synchonisation point to stop thread Semaphore protection mutex
        pthread_mutex_t sync_stop_mutex;

        /**}@*/

    public:
        Thread(string = "");
        virtual ~Thread();

        /**
         * Return the string name
         * @return
         */
        string getName();

        /**
         * Starts the Thread
         */
        void start();

        /**
         * Stops the threas using the synchronisation point
         */
        void stop();

        /**
         * Kills the Thread
         */
        void finish();

        /**
         * Wait for this thread to be finished
         */
        void join();

        /**
         * run method which is ran by the thread
         */
        virtual void run() = 0;

        /**
         * This method blocks until the sync_started sync point has been released
         */
        void joinStarted();

    protected:

        /**
         * Checks on the sync_stop sync point for run() method logic to know if it should stop
         * Stop is also triggered if there is an error with the sync point
         * @return false if we can continue, true otherwise
         */
        bool stopRequested();

        /**
         * Posts grants in the sync_started semaphore to signal the logic is started.
         * This is typically used in the run() method to signal the thread is ready to do its job
         *
         * @return
         */
        void signalStarted();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* THREAD_H_ */
