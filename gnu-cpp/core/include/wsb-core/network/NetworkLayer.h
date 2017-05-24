/*
 * NetworkLayer.h
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

#ifndef NETWORKLAYER_H_
#define NETWORKLAYER_H_

// Includes
//-----------------------

//-- Std
#include <list>
#include <map>
using namespace std;

//-- Common
#include <wsb-core/common/Lifecycle.h>

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Broker
#include <wsb-core/broker/Broker.h>

//-- Network
#include <wsb-core/network/MessagePoller.h>
#include <wsb-core/network/AbstractConnector.h>

namespace OSI {
namespace WSB {

/**
 * This is the network layer, starting/stoping/managing the connectors and poller
 */
class NetworkLayer: public Lifecycle {

    private:

        /// Pollers, per default one
        list<MessagePoller> pollers;

        /// Connectors map: uri <-> Connector *
        map<string, AbstractConnector*> connectors;

        /// Reference to Broker for propagating messages
        /// This class doesn't manage the pointed instance
        Broker * broker;

    public:
        NetworkLayer();
        virtual ~NetworkLayer();

        /**
         * Send a message
         * @param
         */
        void send(MessageContext *);

        /**
         * Adds a connector to the Network layer
         * @warning May not be called on started lifecycle state
         * @param connector
         */
        void addConnector(AbstractConnector * connector);

        /**
         * Returns the reference to the registered Broker.
         * @return if null, no real broking of messages will be performed
         */
        Broker * getBroker();

        /**
         * Set the broker instance to broker messages.
         * @warning May not be called on started lifecycle state
         * @param
         */
        void setBroker(Broker *);

        /** \defgroup Lifecycle */
        /** @{ */

        /**
         *
         */
        void init();

        /**
         * Start the network Layer
         */
        void start();

        /**
         *
         */
        void stop();

        /**
         *
         */
        void reload();

        /**
         *
         */
        void finish();

        /** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* NETWORKLAYER_H_ */
