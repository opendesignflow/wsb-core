/*
 * DOMManipulator.h
 *
 *  Created on: Nov 9, 2011
 *      Author: rleys
 */

#ifndef DOMMANIPULATOR_H_
#define DOMMANIPULATOR_H_

// Includes
//-------------------

//-- Std
#include <string>
#include <list>
using namespace std;

//-- VDOM
#include <vxml/vdom/VDOMDocument.h>
#include <vxml/vxpath/VXPathNSContext.h>

//-- Add std::string support for string<->XML string transitions
#define S2X(str) X(str.c_str())
#define X2S(xml) string(UTF8(xml))

namespace OSI {
namespace WSB {

/**
 *
 *
 * \brief
 * This class is a utility to gather all methods to easily manipulate XML and switch implementations if needed
 * It will help keep code clear.
 *
 * It supports Xerces-C and Xalan-C for the moment
 *
 * \details
 * Once a document is wrapped in the Manipulator, only it should be used to manage the Document life
 *
 */
class DOMManipulator {

protected:

	/// The Document managed by the manipulator
	VDOM::VDOMDocument * document;

	/// Standalone NS Resolver for Xpath
	VDOM::VXPathNSContext  nsContext;

	/// This is the base Implementation of XQuery expression executor
	//XQC_Implementation * xqueyImpl;

public:

	/**
	 * Creates a DOMManipulator with a default DOM Document
	 */
	DOMManipulator();
	DOMManipulator(VDOM::VDOMDocument *);
	virtual ~DOMManipulator();


	/**
	 * Return this manipulated document
	 * @return
	 */
	VDOM::VDOMDocument * getDocument();

	/**
	 * Returns the NS Resolver
	 * @return Returns a reference because DOMManipulator is owner of the pointer
	 */
	VDOM::VXPathNSContext & getNSResolver();


	/** \defgroup Document */
	/** @{ */

	/**
	 * Serialize to string the manipulated document
	 * @return
	 */
	string toString();


	/** @} */




	/** \defgroup Elements Elements Search/Update Methods */
	/** @{ */


	/**
	 * Add a new Element to the root of the document
	 * If no document element is present, the created element is set as the document element
	 * @param ns
	 * @param name
	 * @return The create DOM Element
	 */
	VDOM::VDOMElement * addElement(string ns,string name);

	/**
     * Creates and element with provided content in the given base (Document/Document Element if NULL)
     * If such an element already exists, then it will be updated, otherwise created
     * @param ns
     * @param name
     * @param textContent
     * @param base
     * @return
     */
	VDOM::VDOMElement * addUniqueElement(string ns,string name,string textContent = "",VDOM::VDOMNode * base=NULL);

	/**
	 * Creates a new Element owned by the document
	 * @param ns
	 * @param name
	 * @param base Parent element of the created element, or NULL for Document / Document Element
	 * @return
	 */
	VDOM::VDOMElement * createElement(string ns,string name,VDOM::VDOMNode * base=NULL);




	/**
	 * IMPROVE
     * Uses provided XPath to get first element
     * #hasElement is called before to make sure path exists.
     * Not very optimal
	 * @param xpath
	 * @param base The base node on which to start the research. Document if NULL
	 * @return
	 */
	VDOM::VDOMElement * getElement(string xpath,VDOM::VDOMElement * base = NULL);

	/**
	 * Returns all the found elements for the given XPath expression
	 *
	 * @param xpath
	 * @param base The base context node, or Document if NULL
	 * @return A list containing results DOMElements, or an empty list if nothing was found
	 */
	list<VDOM::VDOMElement*> getElements(string xpath,VDOM::VDOMElement * base = NULL);

	/**
	 * Checks if the provided xpath expression does point to at least an element
	 * @warning The base element is then the Document (not the document element)
	 * @param xpath The xpath
	 * @return
	 */
	bool hasElement(string xpath,VDOM::VDOMElement * base = NULL);



	/** @}*/

	/** \defgroup SimpleTypes Simple Types Resolution */
	/** @{ */


	/**
     * Resolve the provided xpath as a string
     * @param xpath
     * @return
     */
    string getString(string xpath,VDOM::VDOMElement * base = NULL);

	/**
	 * Resolves the provided xpath as a string then as an integer
	 * Throws a runtime_error if the expression resolves to an empty string
	 * @param xpath
	 * @return
	 */
	int getInteger(string xpath,VDOM::VDOMElement * base = NULL);

	/**
	 * Set an attribute from integer value
	 * @param base
	 * @param attributeName
	 * @param value
	 */
	void setAttribute(VDOM::VDOMElement * base,string attributeName,int value);

	/**
	 * Set an attribute from string value
	 * @param base
	 * @param attributeName
	 * @param value
	 */
	void setAttribute(VDOM::VDOMElement * base,string attributeName,string value);

	/** @} */


	/** \defgroup XQuery XQuery Execution methods*/
    /** @{ */

	void xquery(string xquery,VDOM::VDOMElement * base = NULL);

	/** @} */


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* DOMMANIPULATOR_H_ */
