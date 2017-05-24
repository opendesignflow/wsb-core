/*
 * MessagePoller.h
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

#ifndef MESSAGEPOLLER_H_
#define MESSAGEPOLLER_H_

// Includes
//----------------

//-- Std
#include <list>
using namespace std;

//-- Common
#include <wsb-core/common/dispatch/Thread.h>
#include <wsb-core/common/Lifecycle.h>

//-- Network
#include <wsb-core/network/AbstractConnector.h>



namespace OSI {
namespace WSB {

//-- Network
class NetworkLayer;

//-- Broker
class Broker;

/**
 * Thread polling for messages from a set of Connectors
 */
class MessagePoller: public Thread, public Lifecycle {

    protected:

        /// Contains the connectors the Poller will poll for messages
        list<AbstractConnector*> connectors;

        /// Reference to Broker for propagating messages
        /// This class doesn't manage the pointed instance
        Broker * broker;

        /// Reference to the parent NetworkLayer for propagating messages
        /// This class doesn't manage the pointed instance
        NetworkLayer * networkLayer;

    public:
        MessagePoller(NetworkLayer *);
        virtual ~MessagePoller();

        /**
         * Set the broker that will be used by this poller
         * A NULL value (like default) means no brokering is performed
         * @warning Synced agains LIFECYCLE_RUNNING state
         * @param
         */
        void setBroker(Broker *);

        /**
         * Adds the provided connector to the list of polled connectors
         *
         * @warning The connector should be disconnected from any other poller before/after
         * @param connector
         */
        void connect(AbstractConnector * connector);

        /**
         * The Polling implementation
         */
        virtual void run();

        /** \defgroup Lifecycle */
        /**@{*/

        virtual void start();

        virtual void stop();

        virtual void finish();

        /**}@*/

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGEPOLLER_H_ */
