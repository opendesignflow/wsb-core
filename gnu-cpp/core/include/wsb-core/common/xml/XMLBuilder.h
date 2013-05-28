/*
 * XMLBuilder.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef XMLBUILDER_H_
#define XMLBUILDER_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/SGC.h>
#include <wsb-core/common/xml/XMLBuilderException.h>

//-- VDOM
#include <vxml/vdom/VDOMDocument.h>

namespace OSI {
namespace WSB {

/**
 * XMLBuilder is a single frontend interface to build XML documents
 * It is created through the factory, allowing to change the underlying implementation selection for the whole application easily
 */
class XMLBuilder : public Garbageable {

protected:

	/// Stores an Exception if an Error Occurs
	//XMLBuilderException * parsingException;



public:
	XMLBuilder();
	virtual ~XMLBuilder();

	/**
	 * Creates a brand new DOM Document
	 *
	 * @return The DOM Document, to be managed by user
	 * @throw XMLBuilderException If there is an error
	 */
	VDOM::VDOMDocument * newDocument() throw(XMLBuilderException);

	/**
	 * Parse the XML Document contained in the provided string
	 * @param xml
	 * @return a pointer to a Document, to be managed by user
	 * @throw XMLBuilderException If there is an error
	 */
	VDOM::VDOMDocument * parseString(string& xml);

	/**
	 * Serializes the provided document
	 * @param doc
	 * @return
	 */
	string toString(VDOM::VDOMDocument * doc);

	//virtual bool handleError(const DOMError& domError);


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* XMLBUILDER_H_ */
