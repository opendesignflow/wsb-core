/*
 * TCPConnector.h
 *
 *  Created on: Dec 2, 2011
 *      Author: rleys
 */

#ifndef TCPCONNECTOR_H_
#define TCPCONNECTOR_H_

// Includes
//-------------------

//-- Std
#include <string>
using namespace std;

//-- Network
#include <wsb-core/network/AbstractConnector.h>
#include <wsb-core/network/ConnectorException.h>

// Defines
//--------------

#define CONNECTOR_TCP_DEFAULT_PORT 8087

namespace OSI {
namespace WSB {

class TCPConnector: public OSI::WSB::AbstractConnector {

protected:

    /// The socket descriptor
    int socketDescriptor;

    /// The socket type (INET or LOCAL)
    int socketType;

    /// The Peer Sockets (The established connections)
    /// Maps between port and socket (all incoming clients become a different port, target server also)
    map<int,int> peerSockets;

    /// Semaphore to signal a peer socket has been registered
    sem_t peerSocketCreated;


public:
	TCPConnector(string url = "tcp://127.0.0.1:8087") throw (ConnectorException);
	virtual ~TCPConnector();


	/** \defgroup Receive/Wait interface */
	/** @{ */

	public: virtual void run();


	/** @} */


	/** \defgroup Send interface */
	/** @{ */

	protected:

	    /**
	     * Per default: Open TCP Connection for clients
	     * @param
	     */
	    virtual void maintainConnection(NetworkContext *);
	    virtual void doSend(MessageContext *) = 0;


	/** @} */

    /** \defgroup Receive Receive interface */
    /** @{ */

	    /**
	     * To be implemented by protocol implementation
	     * In there the implementation receives and extract messages
	     *
	     * @warning When this method returns, no receive will be performed anymore
	     *
	     */
	    virtual void doReceive(int socketDescriptor,int port) = 0;

    /** @} */


	/** \defgroup Lifecycle */
	/** @{ */

	public: virtual void init();

	public: virtual void stop();

	public: virtual void finish();

	public: virtual void reload() = 0;


	/** @} */



};

} /* namespace WSB */
} /* namespace OSI */
#endif /* TCPCONNECTOR_H_ */
