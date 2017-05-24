/**
 * XMLNode.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef XMLNODE_H_
#define XMLNODE_H_

// Includes
//-----------------

//-- Core Common XML
#include <wsb-core/common/xml/DOMManipulator.h>

namespace OSI {
namespace WSB {

/**
 * An XML node is a class representing an XML structure, that is supposed to be generated
 * in a standalone document, or as Child of a parent element in an existing document
 */
class XMLNode {

    protected:


        /** Either the DocumentElement of created local document, or the provided base location
         *  The base Element is the "root" of this node, and is created by #initBaseElement
         */
        VDOM::VDOMElement * baseElement;

        /// Created By constructor to manipulate XML easily
        DOMManipulator * manipulator;

    public:

        /**
         * Creates an XML Document and sets inited #baseElement to its DocumentElement
         */
        XMLNode();

        /**
         * This XML Node will be created using the provided baseElement
         * @param parentElement
         */
        XMLNode(VDOM::VDOMElement * baseElement);

        virtual ~XMLNode();

        /**
         * Called by constructor:
         *  - To init the baseElement if we are creating a new Node (baseElement is NULL)
         *  - To verify the provided baseElement node (baseElement != NULL)
         */
        virtual void initBaseElement() =0 ;

        /**
         * Returns this internal DOM manipulator
         * @return
         */
        DOMManipulator & getDOMManipulator();

        /**
         * Serializes the internal DOM Manipulator
         * @return
         */
        virtual string toXMLString();





};

} /* namespace WSB */
} /* namespace OSI */
#endif /* XMLNODE_H_ */
