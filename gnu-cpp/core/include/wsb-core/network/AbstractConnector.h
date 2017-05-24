/*
 * AbstractConnector.h
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

#ifndef ABSTRACTCONNECTOR_H_
#define ABSTRACTCONNECTOR_H_

// Includes
//-------------------

//-- Std
#include <queue>
#include <string>
using namespace std;

//-- Sync
#include <pthread.h>
#include <semaphore.h>

//-- Common
#include <wsb-core/common/dispatch/Thread.h>
#include <wsb-core/common/Lifecycle.h>
#include <wsb-core/common/Logging.h>

//-- Message
#include <wsb-core/message/Message.h>
#include <wsb-core/message/MessageContext.h>

namespace OSI {
namespace WSB {

//-- Common
class URI;

//-- Broker
class BrokeringTree;

//-- Dispatch
template<class T> class SafeBlockingQueue;

/**
 * Base Class for Connectors
 *
 * A connector will be started in a Thread through the Lifecycle interface.
 *
 * So to start a connector, call: start()
 *
 */
class AbstractConnector: public Thread, public Lifecycle {

    public:

        /**
         * Determines the direction of this Connector
         */
        enum DIRECTION {
            CLIENT,SERVER
        };

    protected:

        /// The URI describing this connector
        URI * uri;

        /// Messages FIFO
        /// Always use offer/poll methods to access this queue, as they are the synchonised methods !!!
        SafeBlockingQueue<MessageContext*> * messages;

        /// Intermediary Chain
        BrokeringTree * brokerTree;

        /// Message Type for the factory
        string messageType;

        /// Default: Server
        DIRECTION connectorDirection;

    public:
        AbstractConnector(string uri);
        virtual ~AbstractConnector();

        /**
         * Returns the URI Representing a network connection with this connector
         * @return Pointer to the URI object. Managed by this connector
         */
        URI * getURI();

        /**
         *
         * @return The message type supported by the connector (Ex: soap)
         */
        string getMessageType();

        /**
         * Set the message type for this connector instance (Ex: soap)
         */
        void setMessageType(string messageType);

        /**
         * Return thie actually set BrokerTree
         * @return The BrokerTree instance of NULL if none set
         */
        BrokeringTree * getBrokeringTree();

        /**
         * Set a new BrokerTree to this connector.
         * If a previous Broker Tree exists, it  should be managed/deleted by user
         * @param The Broker Tree to set
         */
        void setBrokeringTree(BrokeringTree *);

        /**
         * Returns the direction (client/server) of the connector
         * @return
         */
        DIRECTION getDirection();

        /**
         * Sets the direction (client/server) of the connector
         * @param direction
         */
        void setDirection(DIRECTION direction);

        /**
         * Builds a Message from a string using a Factory
         * @param message
         * @return
         * @throws MessageBuilderException
         * @throws MessageFactoryNotFound
         */
        Message * buildMessage(string message);

        /**
         * Returns the first available message in the connector fifo
         * This method synchronizes the queue
         *
         * @param waitns The number of nanoseconds the polling will wait for available messages. 0 = Waits indefinitely
         * @return A message context or NULL if none (like there is an error, or the polling timed out)
         */
        MessageContext * poll(long int waitns = 0);

        /** \defgroup Send interface */
        /** @{ */

        /**
         * performs the send of the message context
         *
         * First calls: #maintainConnection(NetworkContext *)
         * Then #doSend(MessageContext *)
         *
         * @param
         */
    public:
        void send(MessageContext *);

        /**
         * Called before a send, to ensure the connection is opened or maintained to the destination
         * @param
         */
    protected:
        virtual void maintainConnection(NetworkContext *) = 0;

        /**
         * Performs the actual send operation after maintainConnection
         */
    protected:
        virtual void doSend(MessageContext *) = 0;

        /** @} */

        /** \defgroup Lifecycle */
        /** @{ */

    public:

        /**
         * Stop Operation
         * Provides a Synchronisation point for run() to detect stop requests
         */
        void stop();

        /**
         * Performs some initiations
         * At this level, only progess to stopped
         */
        virtual void init();

        /**
         * Reload The connector
         */
        virtual void reload() = 0;

        /**
         * Call redirection to Thread
         */
        void start();

        /**
         * Call Redirection to Thread
         */
        void finish();

        /** @} */

    protected:


        /**
         * Returns A logger with baseString+(client/server) depending on connector instanciation
         * @param baseString
         * @return
         */
        log4cpp::Category * getLogger(string baseString);

        /**
         * Offers a message to be added to the queue
         * The method creates the message context for the user
         *
         * @param message The message
         * @param context The Network context
         */
        void offer(Message * message, NetworkContext * context);

        /**
         * Runs  the connector through a Thread
         */
        virtual void run() = 0;

};

} // namespace WSB
} // namespace OSI
#endif /* ABSTRACTCONNECTOR_H_ */
