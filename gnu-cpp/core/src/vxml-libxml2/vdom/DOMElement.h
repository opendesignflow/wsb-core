/**
 * DOMElement.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef DOMELEMENT_H_
#define DOMELEMENT_H_

// Includes
//-----------------

//-- VDOM
#include <vxml/vdom/VDOMElement.h>


//-- LIBXML2 Implementation
#include "DOMNode.h"

namespace VDOM {
namespace LIBXML2 {

/**
 * Implementation of DOMElement for libxml2
 */
class DOMElement: public VDOM::VDOMElement , public DOMNode {

    protected:



    public:
        DOMElement(xmlNodePtr nodePrt,DOMDocument * ownerDocument);
        virtual ~DOMElement();


        /**
         * @see VDOM::VDOMNode
         * @param recursive
         * @return
         */
        virtual VDOMNode * cloneNode(bool recursive);

        /** \defgroup Name Element Name access */
        /** @{ */

        /**
         * @see VDOM::VDOM::VDOMElement
         * @return
         */
        string getLocalName();

        /**
         * @see VDOM::VDOMElement
         * @return
         */
        string getName();

        /**
         * @see VDOM::VDOM::VDOMElement
         * @return An empty string if the element does not have any defined namespace
         */
        string getNamespaceURI();

        /**
         * @see VDOM::VDOM::VDOMElement
         * @return An empty string if no prefix is defined
         */
        string getPrefix();

        /** @} */

        /** \defgroup Children Children Elements access */
        /** @{ */

        /**
         * @see VDOM::VDOMElement#getElementsByTagName
         * @param name
         * @return
         */
        virtual VDOMNodeList* getElementsByTagName(string name) ;


        /**
         * @see VDOM::VDOMElement#getElementsByTagNameNS
         * @param namespaceURI
         * @param localname
         * @return
         */
        virtual VDOMNodeList* getElementsByTagNameNS(string namespaceURI,string localname);

        /** @} */

        /** \defgroup Attributes Attributes access */
        /** @{ */

        /**
         * @see VDOM::VDOMElement#getAttribute
         * @param name
         * @return
         */
        virtual bool hasAttribute(string name);

        /**
         * @see VDOM::VDOMElement#getAttribute
         * @param name
         * @return
         */
        virtual string getAttribute(string name);

        /**
         * @see VDOM::VDOMElement#getAttribute
         * @param name
         * @param value
         */
        virtual void setAttribute(string name,string value);

        /** @} */


        /** \defgroup Structure Tree Structure methods */
        /** @{ */

        /**
         * @see VDOM::VDOMNode
         * @return
         */
        virtual VDOMDocument * getOwnerDocument();

        /**
         * @see VDOM::VDOMNode
         * @param newChild
         */
        virtual void appendChild(VDOMNode * newChild);

        /**
         * @see VDOM::VDOMNode
         * @return
         */
        virtual VDOMNodeList * getChildNodes();

        /**
         * @see VDOM::VDOMNode
         * @return
         */
        virtual VDOMNode * getParentNode();

        /** @} */

        /** \defgroup Content Element content manipulation */
        /** @{ */

        /**
         * @see VDOM::VDOMElement
         * @param content
         */
        virtual void setTextContent(string& content);

        /**
         * @see VDOM::VDOMElement
         * @return
         */
        virtual string getTextContent();

        /** @} */


        /** \defgroup Type Node Type methods */
        /** @{ */

        /**
         * Inserts the given newNode before the childNode of this current element
         * @param newNode
         * @param childNode
         */
        virtual void insertBefore(VDOMElement * newNode,VDOMElement * childNode);

        /** @} */

};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* DOMELEMENT_H_ */
