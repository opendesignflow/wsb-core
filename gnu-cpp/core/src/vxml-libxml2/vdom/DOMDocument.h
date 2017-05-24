/**
 * DOMDocument.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef DOMDOCUMENT_H_
#define DOMDOCUMENT_H_

// Includes
//-----------------

//-- Lib XML2
#include "DOMNode.h"
#include <libxml2/libxml/parser.h>

//-- VDOM
#include <vxml/vdom/VDOMDocument.h>
#include <vxml/vdom/VDOMElement.h>
#include <vxml/vdom/utils/NodePool.h>

namespace VDOM {
namespace LIBXML2 {

class DOMDocument: public VDOM::VDOMDocument , public DOMNode {

    private:

       /// This is a node pool to ensure all node have only one created object instance
       NodePool<xmlNodePtr> nodePool;

    protected:

        /// Pointer to the document
        xmlDocPtr doc;

    public:
        DOMDocument(xmlDocPtr doc);
        virtual ~DOMDocument();

        /** \defgroup Implementation Implementation Specific methods */
        /** @{ */

        xmlDocPtr& getDocumentPointer();

        /**
         * Returns this document's node pool. Used mainly by DOMNode::toNode to ensure pointers caching
         * @return
         */
        NodePool<xmlNodePtr>& getNodePool();

        /** @} */

        /**
         * @see VDOM::VDOMNode
         * @param recursive
         * @return
         */
        virtual VDOMNode * cloneNode(bool recursive);

        /** \defgroup XPath XPAth Methods */
        /** @{ */

        virtual VXPathContext * createXPathContext(VDOMElement * baseElement = NULL);

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


        /** \defgroup Elements Elements Access Methods */
        /** @{ */

        /**
         * @see VDOM::VDOMDocument#getDocumentElement()
         * @return
         */
        virtual VDOMElement * getDocumentElement() ;

        /** @} */

        /** \defgroup Creation Content Creation methods */
        /** @{ */

        /**
         *  @see VDOM::VDOMDocument
         * @param nsURI
         * @param qname
         * @return
         */
        virtual VDOMElement * createElementNS(string& nsURI,string&qname);

        /**
         *  @see VDOM::VDOMDocument
         * @param name
         * @return
         */
        virtual VDOMElement * createElement(string& name)  ;


        /** @} */




};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* DOMDOCUMENT_H_ */
