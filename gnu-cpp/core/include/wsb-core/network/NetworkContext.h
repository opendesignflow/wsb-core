/*
 * NetworkContext.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef NETWORKCONTEXT_H_
#define NETWORKCONTEXT_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/URI.h>

namespace OSI {
namespace WSB {

/**
 *
 * The Network Context contains enough information to be able to (re)route messages to the right destination(right connector, right endpoint)
 *
 */
class NetworkContext {

    public:

        /**
         * This enum is used to describe the state of the current Network Context
         * For example, a DEAD state may need a socket need to be connected, while ALIVE means it is already connected
         */
        enum NetworkState {

            DEAD,//!< DEAD
            ALIVE//!< ALIVE

        };

    protected:

        /// An URI to find back/create connectors
        URI * uri;

        /// The current network state. Defaults to DEAD
        NetworkContext::NetworkState networkState;

    public:
        NetworkContext(string uri);
        virtual ~NetworkContext();

        /**
         * Return the URI of this NWC
         * @return
         */
        URI * getURI();

        /**
         * Returns the current state of this network context
         * @return
         */
        NetworkContext::NetworkState getNetworkState();

        /**
         * Change the current state of this network context
         * @param networkState
         */
        void setNetworkState(NetworkContext::NetworkState networkState);

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* NETWORKCONTEXT_H_ */
