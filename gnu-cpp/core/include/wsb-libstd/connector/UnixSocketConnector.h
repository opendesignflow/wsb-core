/*
 * UnixSocketConnector.h
 *
 *  Created on: Nov 22, 2011
 *      Author: rleys
 */

#ifndef UNIXSOCKETCONNECTOR_H_
#define UNIXSOCKETCONNECTOR_H_

// Includes
//---------------

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Network
#include <wsb-core/network/AbstractConnector.h>

//-- Unix socket
#include <sys/types.h>
#include <sys/socket.h>

namespace OSI {
namespace WSB {


/**
 * Not sure what to do
 */
class UnixSocketConnector : public AbstractConnector {

protected:

	/// The socket descriptor
	int socketDescriptor;

public:
	UnixSocketConnector();
	virtual ~UnixSocketConnector();


	/** \defgroup Implementation */
	/** @{ */

public:


	/** \defgroup Send interface */
	/** @{ */

	/**
	 * @see AbstractConnector#maintainConnection
	 */
	protected: virtual void maintainConnection(NetworkContext *);

	/**
	 * @see AbstractConnector#doSend
	 */
	protected: virtual void doSend(MessageContext *);

	/**@}*/

public:
	/**
	 * Performs some initiations
	 */
	virtual void init();

	/**
	 * Reload The connector
	 */
	virtual void reload();

	/**
	 * Stop the socket and the thread
	 */
	virtual void stop();

	/**
	 * Stop the socket and finish the thread
	 */
	virtual void finish();

protected:



	virtual void run();


	/** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* UNIXSOCKETCONNECTOR_H_ */
