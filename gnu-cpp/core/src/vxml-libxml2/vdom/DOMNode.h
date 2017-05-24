/**
 * DOMNode.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef DOMNODE_H_
#define DOMNODE_H_

// Includes
//-----------------

//-- Std
#include <stdexcept>
using namespace std;

//-- VDOM
#include <vxml/vdom/VDOMNode.h>
#include <vxml/vdom/utils/NodePool.h>

//-- Libxml2
#include <libxml/tree.h>




namespace VDOM {
namespace LIBXML2 {

//-- Forward Declaration to avoid cyclic declaration
class DOMDocument;

class DOMNode {

    private:



    protected:

        /// This is the libxml2 node pointer
        xmlNodePtr nodePtr;

        /// Pointer on Owner document
        DOMDocument * document;



    public:
        DOMNode(xmlNodePtr node,DOMDocument * ownerDocument);

        /**
         * This destructor  frees the xmlNodePtr
         */
        virtual ~DOMNode();


        /**
         * @see VDOM::VDOMNode
         * @param recursive
         * @return
         */
        virtual VDOMNode * cloneNode(bool recursive);

        /**
         * Converts the provided xmlNodePtr to a corresponding VDOMNode type
         * For example, if  node->type == XML_ELEMENT_NODE , this will return a new DOMElement
         * If the node type is not supported, NULL is returned
         * @param node
         * @return
         * @throw an invalid_argument is the provided pointer is null
         */
        static VDOMNode * toNode(xmlNodePtr node,DOMDocument * document) throw (invalid_argument);

        /**
         * This method checks the provided pointer is part of the Libxml2 implementation
         * If not, it throws an invalid_argument exception
         * @param node
         */
        static void assertLIBXML2VDOM(VDOMNode * node) throw (invalid_argument);

        /**
         * Returns the node pointer for this node
         * @return
         */
        xmlNodePtr getNodePointer();

        /** \defgroup Structure Tree Structure methods */
        /** @{ */

        /**
         * @see VDOM::VDOMNode
         * @return
         */
        virtual VDOMNodeList * getChildNodes();


        /**
         * @see VDOM::VDOMNode
         * @return
         */
        virtual VDOMDocument * getOwnerDocument();

        /**
         * @see VDOM::VDOMNode
         * @return
         */
        virtual VDOMNode * getParentNode();


        /** @} */

};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* DOMNODE_H_ */
