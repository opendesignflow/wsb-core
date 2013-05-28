/*
 * WSEngine.h
 *
 *  Created on: Nov 14, 2011
 *      Author: rleys
 */

#ifndef WSENGINE_H_
#define WSENGINE_H_

// Includes
//----------------------------

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/Lifecycle.h>

//-- Network
#include <wsb-core/network/NetworkLayer.h>

//-- Broker
#include <wsb-core/broker/Broker.h>

namespace OSI {
namespace WSB {

/**
 * The Engine instanciates and starts all the components of the message broker.
 * It also provides the interfaces to access all layers
 */
class WSEngine: public Lifecycle {

    protected:

        /// Network Layer
        NetworkLayer network;

        /// Message Broker
        Broker broker;

    public:
        WSEngine();
        virtual ~WSEngine();

        /**
         * Return the Network Layer
         * @return
         */
        NetworkLayer& getNetwork();

        /**
         * Return the Broker
         * @return
         */
        Broker& getBroker();

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
#endif /* WSENGINE_H_ */
