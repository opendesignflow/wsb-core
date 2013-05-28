/**
 * NodePool.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef NODEPOOL_H_
#define NODEPOOL_H_

// Includes
//-----------------

//-- Std
#include <map>
#include <omp.h>
#include <iostream>
using namespace std;

#include "../VDOMNode.h"

namespace VDOM {

/**
 * The node pool is simply a thread safe mapping class to store created instances of VDOM object, and recover them later.
 * The one template parameter is used to specify a data type provided by the implementation XML library, and  that can be used as
 * a mapping key
 *
 * @warning The NodePool does not delete all the pooled pointer
 *
 */
template<class T> class NodePool {

    protected:

        /// This is the storage map
        map<T,VDOMNode*> poolMap;

        /// This lock is used to make the Pool thread-safe
        omp_nest_lock_t  threadLock;

    public:
        NodePool() {

            //-- Init Lock
            omp_init_nest_lock(&(this->threadLock));

        }

        /**
         * @warning The NodePool does not delete all the pooled pointer
         */
        virtual ~NodePool() {

            this->poolMap.clear();
            omp_destroy_nest_lock(&(this->threadLock));

        }

        /**
         * Simply returns a reference to the underlying map
         * @return
         */
        map<T,VDOMNode*>& getMap() {
            return this->poolMap;
        }



        /**
         * Puts the new node in the map for the key.
         * If the key already exists, returns the already set value, @node otherwise
         *
         * The application should look at the return value, and if not @node, delete the node that it tried to write, and only use the one returned
         *
         * @param key
         * @param
         * @return NULL if the key has been written, the actual value if alredy existing
         */
        VDOMNode * put(T key,VDOMNode * node) {

            //-- Lock
            omp_set_nest_lock(&(this->threadLock));

            VDOMNode * result = node;

            //-- Write if nothing, otherwise return already existing value. This is used to resolve double write race conditions
            if (this->poolMap[key]==NULL) {
                this->poolMap[key] = node;
            } else
                result = this->poolMap[key];


            //-- Unlock
            omp_unset_nest_lock(&(this->threadLock));

            return result;

        }

        VDOMNode * get(T key) {

            //-- Lock
            omp_set_nest_lock(&(this->threadLock));


            //VDOMNode * result = 0;

            //-- Non Read-Blocking locking
            //--------------

            //-- test
           /* int thread_lock_count = omp_test_nest_lock(&(this->threadLock));

            //-- If lock_count >0 we have the lock -> release immediately
            if (thread_lock_count>0)
                omp_unset_nest_lock(&(this->threadLock));
            //-- If lock_count == 0 -> Somebody writing / test_reading has the lock, wait for it
            else {
                omp_set_nest_lock(&(this->threadLock));
                omp_unset_nest_lock(&(this->threadLock));
            }*/


            //-- Read
            //-----------------

            VDOMNode * result = this->poolMap[key];

            //-- Unlock
            omp_unset_nest_lock(&(this->threadLock));

            return result;

        }
};

} /* namespace VDOM */
#endif /* NODEPOOL_H_ */
