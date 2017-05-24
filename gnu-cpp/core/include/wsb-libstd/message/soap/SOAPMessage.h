/*
 * SOAPMessage.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef SOAPMESSAGE_H_
#define SOAPMESSAGE_H_

// Includes
//-----------------

//-- Std
#include <iostream>
using namespace std;

//-- XML
#include <wsb-core/common/xml/DOMManipulator.h>

//-- Message
#include <wsb-core/message/Message.h>

//-- A Few Defines
#define NS_SOAP "http://www.w3.org/2001/12/soap-envelope"


namespace OSI {
namespace WSB {

/**
 * This represents a SOAP Message
 *
 * The base type of a SOAP Message is a DOMManipulator
 */
class SOAPMessage: public Message {

public:
	/**
	 * Creates a Message and initialises its basic structure in a DOMManipulator
	 */
	SOAPMessage();
	SOAPMessage(DOMManipulator * );
	virtual ~SOAPMessage();




	/**
	 * Returns the base of this message : A DOMMAnipulator
	 * @warning The Base is managed by the message. Don't delete it yourself
	 * @return pointer to DOMManipulator
	 */
	DOMManipulator * getBase();

	/** \defgroup XML Utilities */
	/** @{ */

	/**
	 * Add a NS mapping for the XPath resolver of DOM Manipulator
	 * @param prefix
	 * @param ns
	 */
	void addXPathNamespace(string prefix,string ns);

	/** @} */


	/** \defgroup Header Utilities */
	/** @{ */

    /**
     * Returns the Header element of SOAP message
     *
     * @return
     */
    VDOM::VDOMElement * getHeader();

	/**
	 * Add a header element with a textual content
	 * @param ns Namespace
	 * @param name Name
	 * @param text Text content of new Element
	 * @return A pointer to the Element
	 */
	VDOM::VDOMElement * addHeader(string ns,string name,string text);

	/**
	 * Add a header element with no textual content
	 * @param ns
	 * @param name
	 * @return
	 */
	VDOM::VDOMElement * addHeader(string ns,string name);

	/**
	 * Tries to find a Header element using the provided XPath expression
	 * @param xpath An xpath expression that will be concatenated with: "/S:Envelope/S:Header/"
	 * @return
	 */
	VDOM::VDOMElement * getHeaderElement(string xpath);


	/** @} */

	/** \defgroup Body Utilities */
    /** @{ */

	/**
     * Returns the Body element of SOAP message
     *
     * @return
     */
	VDOM::VDOMElement * getBody();

    /** Adds a new Element in the S:Envelope/S:Body of the SOAP message
     *
     * @param ns Namespace of the element
     * @param name Qualified Name (prefix:local-name)
     * @param text A Text content to add
     * @return
     */
	VDOM::VDOMElement * addBodyPayload(string ns,string name,string text);

    /** Adds a new Element in the S:Envelope/S:Body of the SOAP message
     *
     * @param ns Namespace of the element
     * @param name Qualified Name (prefix:local-name)
     * @return
     */
	VDOM::VDOMElement * addBodyPayload(string ns,string name);


	/** @} */


	virtual void formatError(IntermediaryException *);

	/**
	 * Serialize out
	 * @param
	 * @return
	 */
	string toString();


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* SOAPMESSAGE_H_ */
