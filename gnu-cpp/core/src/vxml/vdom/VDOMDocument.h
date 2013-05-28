/**
 * VDOMDocument.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMDOCUMENT_H_
#define VDOMDOCUMENT_H_

// Includes
//-----------------

//-- VDOM
#include "VDOMNode.h"
#include "VDOMElement.h"

//-- VXPath
#include "../vxpath/VXPathContext.h"

namespace VDOM {

/**
 * This is an interface class for a DOMDocument
 */
class VDOMDocument : public VDOMNode {



    public:
        VDOMDocument();
        virtual ~VDOMDocument();

        virtual VDOMNode::NodeType getNodeType();

        /** \defgroup XPath XPAth Methods */
        /** @{ */

        /**
         * Creates an XPAth Context based on this document.
         * The origin is either the document, or the provided baseElement if not NULL
         * @warning The returned pointer is owned by the user
         * @return
         */
        virtual VXPathContext * createXPathContext(VDOMElement * baseElement = NULL) = 0;

        /** @} */


        /** \defgroup Elements Elements Access Methods */
        /** @{ */

        /**
         * Returns the document element
         * @return
         */
        virtual VDOMElement * getDocumentElement() = 0 ;

        /** @} */


        /** \defgroup Creation Content Creation methods */
        /** @{ */

        /**
         * Creates a new element having provided Qualified name and namespace URI
         * @param nsURI
         * @param qname
         * @return
         */
        virtual VDOMElement * createElementNS(string& nsURI,string&qname) = 0 ;

        /**
         * Creates a new element with provided non namespaced simple name
         * @param name
         * @return
         */
        virtual VDOMElement * createElement(string& name) = 0 ;


        /** @} */

};

} /* namespace VDOM */
#endif /* VDOMDOCUMENT_H_ */
