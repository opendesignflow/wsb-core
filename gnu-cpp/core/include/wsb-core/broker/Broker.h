/*
 * Broker.h
 *
 *  Created on: Nov 14, 2011
 *      Author: rleys
 */

#ifndef BROKER_H_
#define BROKER_H_

// Includes
//---------------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Lifecycle.h>
#include <wsb-core/common/dispatch/ParallelDispatchQueue.h>

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>

namespace OSI {
namespace WSB {

//-- Network
class NetworkLayer;

/**
 * The broker is the class maintaining and launching concrete message brokering actions
 *
 * FIXME Brokered MessageContext have to be checked for end of brokering and deleted
 *
 */
class Broker: public Lifecycle {

    protected:

        /// The main brokering Tree. Managed by this Class!
        /// As pointer to enable easy reinit
        BrokeringTree * brokeringTree;

        /// Parallel Workqueue to submit Brokering jobs
        ParallelDispatchQueue workqueue;

    public:
        Broker();
        virtual ~Broker();

        /** \defgroup Brokering Message brokering */
        /** @{ */

        /**
         * Broker an input message
         * @param
         */
        void broker(NetworkLayer *, MessageContext *);

        /**
         * Broker UP an input message (to send from client point of view for example)
         * @param
         */
        void send(NetworkLayer * networkLayer, MessageContext *);

        /** @} */

        /** \defgroup Brokering Tree Manipulation */
        /** @{ */

        /**
         * Returns the actual Brokering Tree
         * @warning The BrokeringTree pointer is managed by this class
         * @return null after init.
         */
        BrokeringTree * getBrokeringTree();

        /**
         * Sets a new BrokeringTree
         * If A tree already exists, it gets deleted
         * @warning The BrokeringTree pointer is managed by this class
         * @param
         */
        void setBrokeringTree(BrokeringTree*);

        /**
         * Set a new  brokering tree
         * @param xml
         */
        void readInFromString(string xml);

        /**
         * Set a new brokering Tree from a file
         * @param filepath
         */
        void readInFromFile(string filepath);

        /** @} */

        /** \defgroup Lifecycle */
        /** @{ */

        virtual void init();

        virtual void start();

        virtual void reload();

        virtual void stop();

        virtual void finish();

        /** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* BROKER_H_ */
