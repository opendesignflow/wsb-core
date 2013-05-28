/**
 * Message.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

// Includes
//---------------

//-- Std
#include <string>
using namespace std;

namespace OSI {
namespace WSB {

//-- Broker
class IntermediaryException;

/**
 * Represents a Message
 * This class is abstract and should be implemented by subclasses to support multiple message types
 *
 */
class Message {


protected:

	/// The message type
	string messageType;

	/// The messageQualifier
	/// Used to filter message passage through Intermediaries tree
	string messageQualifier;

	/// The base object used to manage this message
	/// Ex: A DOM Document for SOAP
	void * base;

public:
	Message();
	virtual ~Message();

	/**
	 * Returns the base object used for this message.
	 * Ex: A DOM Document for SOAP
	 * @return the templated base object
	 */
	//virtual template <class T> T * getBase();


	/**
	 * This message represents en error, and should format itself to represent it
	 * @param
	 */
	virtual void formatError(IntermediaryException *) = 0;

	/**
	 * Returns the type of the message implementation
	 * Ex: SOAP
	 * @return string
	 */
	 string getType();

	 /**
	  * Returns the qualifier used to filder the brokering tree
	  * ex: my.message.action
	  * @return An empty string if no Qualifier or Undefined
	  */
	 string getQualifier();

	 /**
	  * Sets the qualifier of this message
	  * @param qualifier
	  */
	 void setQualifier(string qualifier);

	 /**
	  * Serialise out
	  * @return
	  */
	 virtual string toString();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGE_H_ */
