/*
 * MessageFactory.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef MESSAGEFACTORY_H_
#define MESSAGEFACTORY_H_

// Includes
//--------------

//-- Std
#include <string>
using namespace std;

//-- Message
#include <wsb-core/message/Message.h>
#include <wsb-core/message/MessageFactoryNotFound.h>
#include <wsb-core/message/MessageBuilderException.h>

namespace OSI {
namespace WSB {

/**
 * This class is to be derived by MessageFactories building a specific type of message
 */
class MessageFactory {
public:
	MessageFactory();
	virtual ~MessageFactory();

	/**
	 * Create a Message Factory for the provided message type
	 * @param messageType
	 * @return
	 * @throws MessageFactoryNotFound if not found
	 */
	static MessageFactory * getInstance(string messageType)  throw (MessageFactoryNotFound);

	/**
	 * Build a message from a string
	 *
	 * @param message
	 * @return NULL if not overriden (not virtual because of template)
	 */
	/*template<class T=Message> T * build(string message) throw (MessageBuilderException) {
		throw MessageBuilderException("Method not implemented in Factory");
	}*/

	virtual Message * build(string message) throw (MessageBuilderException) = 0;

	/**
	 * Build a new Empty message ready to be manipulated
	 * @return (not virtual because of template)
	 */
	/*template<class T=Message> T * build() throw (MessageBuilderException) {
		throw MessageBuilderException("Method not implemented in Factory");
	}*/
	virtual Message * build() throw (MessageBuilderException) = 0;

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGEFACTORY_H_ */
