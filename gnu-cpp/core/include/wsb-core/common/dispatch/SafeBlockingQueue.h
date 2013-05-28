/*
 * SafeQueue.h
 *
 *  Created on: Nov 22, 2011
 *      Author: rleys
 */

#ifndef SAFEBLOCKINGQUEUE_H_
#define SAFEBLOCKINGQUEUE_H_

// Includes
//----------------

//-- Std
#include <queue>
#include <unistd.h>
#include <cmath>
using namespace std;

//-- Dispatch
#include <wsb-core/common/dispatch/MutexLocker.h>

//-- Sync
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

namespace OSI {
namespace WSB {

/**
 * For now just a Thread safe waiting fifo
 *
 * Overriden operators up to now:
 *
 *  - None
 *
 * @warning ONLY USE offer/getBlocking interface, not standard queue for the moment!!
 *
 */
template<class T> class SafeBlockingQueue: public queue<T> {

    protected:

        /// R/W Protec Mutex
        pthread_mutex_t rwMutex;

        /// Data Presence semaphore
        sem_t offerSemaphore;

    public:
        SafeBlockingQueue() :
                queue<T>() {

            //-- Init Sync
            sem_init(&(this->offerSemaphore), 0, 0);
            this->rwMutex = PTHREAD_MUTEX_INITIALIZER;

        }
        virtual ~SafeBlockingQueue() {

            //-- Destroy
            sem_destroy(&(this->offerSemaphore));
            pthread_mutex_destroy(&(this->rwMutex));

        }

        /**
         * Thread safe writing into
         * @param value
         */
        void offer(T value) {

            //-- Write
            MutexLocker rwLock(&(this->rwMutex));
            this->push(value);

            //-- Post semaphore
            sem_post(&(this->offerSemaphore));
        }

        /**
         * This method blocks on a semaphore until a value has been offered through #offer()
         * @param wait A wait time in nano seconds . If 0, wait undefinetely
         * @warning If the queue is deleted, this method unblocks returning NULL, this allows to liberate threads waiting and have them closed nicely
         * @return The front value of this queue (The value is removed from the queue) or NULL if the operation has to stop for example
         */
        T getBlocking(long int waitns = 0) {

            //-- Wait on semaphore
            int res = -1;
            if (waitns == 0 ) {
                res = sem_wait(&(this->offerSemaphore));
            } else {

                //struct timespec waitTime;
                struct timespec tm;
                clock_gettime(CLOCK_REALTIME, &tm);

                //tm.tv_nsec += waitTime.tv_nsec;
                //tm.tv_sec +=  waitTime.tv_sec;

                tm.tv_sec += floor((waitns/(1000*1000*1000)));
                tm.tv_nsec += (waitns%(1000*1000*1000));

                res = sem_timedwait(&(this->offerSemaphore),&tm);
            }

            if (res!=0) {
                //-- An error occured, -> return NULL
                return NULL;
            }

            //-- Read
            T val = this->front();

            //-- Pop (protect while poping)
            MutexLocker rwLock(&(this->rwMutex));
            this->pop();

            return val;
        }

    };

    } /* namespace WSB */
    } /* namespace OSI */
#endif /* SAFEQUEUE_H_ */
