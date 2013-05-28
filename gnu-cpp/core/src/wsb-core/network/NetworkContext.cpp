/*
 * NetworkContext.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//-----------------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/URI.h>
#include <wsb-core/common/Exception.h>

#include "NetworkContext.h"

namespace OSI {
namespace WSB {

NetworkContext::NetworkContext(string uri) {

    //-- Default network state: DEAD
    this->networkState = NetworkContext::DEAD;

    //-- Record URI
    this->uri = new URI(uri);
    if (!this->uri->isValid()) {
        throw Exception("Provided URI "+uri+" is not valid");
    }


}

NetworkContext::~NetworkContext() {
	// TODO Auto-generated destructor stub
}

URI* NetworkContext::getURI() {
	return this->uri;
}

NetworkContext::NetworkState NetworkContext::getNetworkState() {
    return this->networkState;
}

void NetworkContext::setNetworkState(NetworkContext::NetworkState networkState) {
    this->networkState = networkState;
}

} /* namespace WSB */
} /* namespace OSI */
