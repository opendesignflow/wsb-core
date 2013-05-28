/**
 * VDOMElement.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMELEMENT_H_
#define VDOMELEMENT_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

#include "VDOMNode.h"
#include "VDOMNodeList.h"

namespace VDOM {

/**
 * Base class for the DOM Element access
 */
class VDOMElement: public VDOM::VDOMNode {

    public:
        VDOMElement();
        virtual ~VDOMElement();



        virtual VDOMNode::NodeType getNodeType();


        /** \defgroup Name Element Name access */
        /** @{ */

        /**
         * Returns a string representing the element name, without prefix if there was one
         * @return
         */
        virtual string getLocalName() = 0;

        /**
         * Returns a string representing the qualified name (with prefix if any was defined) of the element
         * @return
         */
        virtual string getName() = 0;

        /**
         * Returns a string representing the namespace URI for this element
         * @return An empty string if the element does not have any defined namespace
         */
        virtual string getNamespaceURI() = 0;

        /**
         * Returns a string representing the prefix defined on this element name's qualified name
         * @return An empty string if no prefix is defined
         */
        virtual string getPrefix() =0 ;

        /** @} */

        /** \defgroup Children Children Elements access */
        /** @{ */

        /**
         * Lists all the child elements of this element, with matching local-name
         * The user is owner of the returned list pointer
         * @param name
         * @return A node list with the result, or an empty node list
         */
        virtual VDOMNodeList* getElementsByTagName(string name) = 0 ;


        /**
         * Lists all the child elements of this element, with matching local-name and namespaceURI
         * The user is owner of the returned list pointer
         * @param namespaceURI
         * @param localname
         * @return A node list with the result, or an empty node list
         */
        virtual VDOMNodeList* getElementsByTagNameNS(string namespaceURI,string localname) = 0;

        /** @} */

        /** \defgroup Attributes Attributes access */
        /** @{ */

        /**
         * Returns true if the named attribute is present on the element
         * @param name
         * @return
         */
        virtual bool hasAttribute(string name) = 0 ;

        /**
         * Returns The requested attribute, or an empty string if none found
         * @param name
         * @return The attribute text value, or empty string if the attribute is not defined
         */
        virtual string getAttribute(string name)= 0;

        /**
         * Sets the value of the requested attribute.
         * An empty value removes the attribute
         * @param name
         * @param value
         */
        virtual void setAttribute(string name,string value)= 0;

        /**
         * Removes the named attribute from element
         * The default implementation of this method calls #setAttribute(name,"");  (with an empty value to cause removal)
         * @param name
         */
        virtual void deleteAttribute(string name);

        /** @} */

        /** \defgroup Content Element content manipulation */
        /** @{ */

        /**
         * Set the text content of this element
         * @param content
         */
        virtual void setTextContent(string& content) =0 ;

        /**
         * Convenience method to go pass the reference limitation of #setTextContent(string&), and allow static declaration usage
         * Default implementation just transfers call to #setTextContent(string&)
         * @param content
         */
        virtual void setTextContent(const char * content);

        /**
         * Returns the text content of this element
         * @return
         */
        virtual string getTextContent() = 0;

        /** @} */


        /** \defgroup Type Node Type methods */
        /** @{ */

        /**
         * Inserts the given newNode before the childNode of this current element
         * @param newNode
         * @param childNode
         */
        virtual void insertBefore(VDOMElement * newNode,VDOMElement * childNode) = 0;

        /** @} */


};

} /* namespace VDOM */
#endif /* VDOMELEMENT_H_ */
