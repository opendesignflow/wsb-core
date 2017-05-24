/**
 * VDOMNode.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMNODE_H_
#define VDOMNODE_H_

// Includes
//-----------------

//-- Std
#include <map>
#include <string>
using namespace std;

namespace VDOM {

//-- Forward declaration to avoid cyclic dependencies
class VDOMNodeList;
class VDOMDocument;

/**
 * Base Node class
 */
class VDOMNode {

    protected:

        /// User data map
        map<string,void*> userData;

    public:

        /**
         * The Node Type enumeration for user to know what kind of VDOMNode he has in hand, and make appropriate casting
         */
        enum NodeType {
              ELEMENT_NODE,               //!< ELEMENT_NODE
              ATTRIBUTE_NODE,             //!< ATTRIBUTE_NODE
              TEXT_NODE,                  //!< TEXT_NODE
              CDATA_SECTION_NODE,         //!< CDATA_SECTION_NODE
              ENTITY_REFERENCE_NODE,      //!< ENTITY_REFERENCE_NODE
              ENTITY_NODE,                //!< ENTITY_NODE
              PROCESSING_INSTRUCTION_NODE,//!< PROCESSING_INSTRUCTION_NODE
              COMMENT_NODE,               //!< COMMENT_NODE
              DOCUMENT_NODE,              //!< DOCUMENT_NODE
              DOCUMENT_TYPE_NODE,         //!< DOCUMENT_TYPE_NODE
              DOCUMENT_FRAGMENT_NODE,     //!< DOCUMENT_FRAGMENT_NODE
              NOTATION_NODE               //!< NOTATION_NODE
        };

        VDOMNode();
        virtual ~VDOMNode();


        /**
         * This method clones the current node, and returns an exact copy
         * @param recursive All sub nodes are cloned also, making the returned node a tree copy of the current one
         * @return
         */
        virtual VDOMNode * cloneNode(bool recursive) = 0;


        /** \defgroup User User data methods */
        /** @{ */

        /**
         * Map the provided pointer to the mapped ky in local user data map
         * @warning Not thread-safe
         * @param key
         * @param dataPointer
         */
        virtual void setUserData(string key,void * dataPointer);

        /**
         * Return the pointer to the data registered for the given map key
         * @warning Not thread-safe
         * @param key
         * @return NULL if no data at given key
         */
        virtual void * getUserData(string key);

        /** @} */



        /** \defgroup Type Node Type methods */
        /** @{ */

        virtual VDOMNode::NodeType getNodeType() = 0;

        /** @}*/



        /** \defgroup Structure Tree Structure methods */
        /** @{ */

        /**
         * Returns a pointer to the owner document
         * @return
         */
        virtual VDOMDocument * getOwnerDocument() = 0;

        /**
         * Adds the newChild node to the current node
         * @param newChild
         */
        virtual void appendChild(VDOMNode * newChild) = 0;



        /**
         * Return all Child nodes, no matter what type they are
         * @warning Returned pointer is owner by user
         * @return
         */
        virtual VDOMNodeList * getChildNodes() = 0;

        /**
         * Returns the parent node of this node
         * @return
         */
        virtual VDOMNode * getParentNode() = 0;

        /** @} */

};

} /* namespace VDOM */
#endif /* VDOMNODE_H_ */
