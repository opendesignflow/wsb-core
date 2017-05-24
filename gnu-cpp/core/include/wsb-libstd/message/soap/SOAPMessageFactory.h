/*
 * SOAPMessageFactory.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef SOAPMESSAGEFACTORY_H_
#define SOAPMESSAGEFACTORY_H_

// Includes
//--------------

//-- Std
#include <string>
using namespace std;

//-- Message
#include <wsb-core/message/MessageFactory.h>
#include <wsb-core/message/MessageBuilderException.h>
#include <wsb-core/message/Message.h>

//-- SOAPMessage
#include <wsb-libstd/message/soap/SOAPMessage.h>

//-- Xerces
//#include <xercesc/sax/SAXParseException.hpp>
//#include <xercesc/sax/ErrorHandler.hpp>
//using namespace xercesc;

namespace OSI {
namespace WSB {

class SOAPMessageFactory : public MessageFactory {

public:

	SOAPMessageFactory();
	virtual ~SOAPMessageFactory();

	/**
	 * Build a message from a string
	 * @param message
	 * @return
	 */
	virtual SOAPMessage * build(string message) throw (MessageBuilderException);


	/**
	 * Build a new Empty message ready to be manipulated
	 * @return
	 */
	virtual SOAPMessage * build() throw (MessageBuilderException);


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* SOAPMESSAGEFACTORY_H_ */
