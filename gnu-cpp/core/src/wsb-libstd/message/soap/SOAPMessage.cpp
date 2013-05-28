/*
 * SOAPMessage.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Logging
#include <wsb-core/common/Logging.h>

//-- Broker
#include <wsb-core/broker/IntermediaryException.h>

//-- XML
#include <wsb-core/common/xml/DOMManipulator.h>


#include "SOAPMessage.h"

namespace OSI {
namespace WSB {


SOAPMessage::SOAPMessage() {

	//-- Set message type
	this->messageType = "soap";

	//-- Create base
	this->base = new DOMManipulator();

	//-- Create SOAP Structure
	this->getBase()->addElement(string(NS_SOAP),"Envelope");
	this->getBase()->addElement(string(NS_SOAP),"Header");
	this->getBase()->addElement(string(NS_SOAP),"Body");

	//-- Add SOAP to NS Resolver
	this->getBase()->getNSResolver()["S"] = NS_SOAP;
}

SOAPMessage::SOAPMessage(DOMManipulator * manipulator) {

	//-- Set message type
	this->messageType = "soap";

	//-- Record base
	this->base = manipulator;


}

SOAPMessage::~SOAPMessage() {
	// TODO Auto-generated destructor stub
}

DOMManipulator * SOAPMessage::getBase() {
	return static_cast<DOMManipulator*>(this->base);
}

void SOAPMessage::addXPathNamespace(string prefix,string ns) {

	this->getBase()->getNSResolver()[prefix] = ns;

}

VDOM::VDOMElement * SOAPMessage::getHeader() {

    return this->getBase()->getElement("/S:Envelope/S:Header");

}

VDOM::VDOMElement * SOAPMessage::addHeader(string ns,string name,string text) {

	//-- Create Element
    VDOM::VDOMElement * headerElement = this->getBase()->createElement(ns,name);
	headerElement->setTextContent(text);


	//-- Add to Header
	VDOM::VDOMElement * header = this->getBase()->getElement("/S:Envelope/S:Header");
	header->appendChild(headerElement);


	return headerElement;
}

VDOM::VDOMElement * SOAPMessage::addHeader(string ns,string name) {


	//-- Create Element
    VDOM::VDOMElement * headerElement = this->getBase()->createElement(ns,name);

	//-- Add to Header
    VDOM::VDOMElement * header = this->getBase()->getElement("/S:Envelope/S:Header");
	header->appendChild(headerElement);

	return headerElement;
}

VDOM::VDOMElement * SOAPMessage::addBodyPayload(string ns,string name,string text) {

    //-- Create Element
    VDOM::VDOMElement * payloadElement = this->getBase()->createElement(ns,name);
    payloadElement->setTextContent(text.c_str());


    //-- Add to Header
    VDOM::VDOMElement * body = this->getBody();
    body->appendChild(payloadElement);


    return payloadElement;
}

VDOM::VDOMElement * SOAPMessage::addBodyPayload(string ns,string name) {


    //-- Create Element
    VDOM::VDOMElement * payloadElement = this->getBase()->createElement(ns,name);


    //-- Add to Header
    VDOM::VDOMElement * body = this->getBody();
    body->appendChild(payloadElement);


    return payloadElement;

}

VDOM::VDOMElement * SOAPMessage::getHeaderElement(string xpath) {


	//-- Call up on DOM Manipulator
    VDOM::VDOMElement * element = this->getBase()->getElement("/S:Envelope/S:Header/"+xpath);
	return element;



}

VDOM::VDOMElement * SOAPMessage::getBody() {

    return this->getBase()->getElement("/S:Envelope/S:Body");

}

void SOAPMessage::formatError(IntermediaryException * ex) {

    // Create a SOAP Fault
    //------------------------------

    //-- Get Body
    VDOM::VDOMElement * body = this->getBody();

    //-- Add SOAP Fault
    VDOM::VDOMElement * fault = this->getBase()->createElement(NS_SOAP,"Fault",body);

    //-- FIXME Add Fault code
    VDOM::VDOMElement * faultcode = this->getBase()->createElement(NS_SOAP,"faultcode",fault);
    faultcode->setTextContent("Client");

    //-- Add information string
    VDOM::VDOMElement * faultstring = this->getBase()->createElement(NS_SOAP,"faultstring",fault);
    faultstring->setTextContent(ex->what());


}


string SOAPMessage::toString() {

	string result = this->getBase()->toString();


	return result;

}


} /* namespace WSB */
} /* namespace OSI */
