/*
 * LoopbackConnector.h
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

#ifndef LOOPBACKCONNECTOR_H_
#define LOOPBACKCONNECTOR_H_

// Includes
//-------------------

//-- Std
#include <string>
using namespace std;

//-- Network
#include <wsb-core/network/AbstractConnector.h>

#include <wsb-core/network/NetworkLayer.h>

namespace OSI {
namespace WSB {

class LoopbackConnector : public AbstractConnector {


public:
	LoopbackConnector();
	virtual ~LoopbackConnector();


	/**
	 * Inject a message string into this connector, which presents it to its output interface as if received
	 * @param message
	 */
	void loopMessage(string message);

	/**
	 * Runs  the connector through a Thread
	 */
	virtual void run();

	/**
	 * Performs some initiations
	 */
	virtual void init();

	/**
	 * Reload The connector
	 */
	virtual void reload();

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


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* LOOPBACKCONNECTOR_H_ */
