/*
 * BrokeringTree.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef BrokeringTree_H_
#define BrokeringTree_H_

// Includes
//----------------

//-- Std
#include <string>
#include <list>
#include <map>
using namespace std;

//-- Dispatch
//#include <wsb-core/common/dispatch/ParallelDispatchQueue.h>
//#include <wsb-core/common/dispatch/SerialDispatchQueue.h>

//-- Broker
#include <wsb-core/broker/Intermediary.h>

//-- XML
//#include <xqilla/xqilla-dom3.hpp>
#include <wsb-core/common/xml/DOMManipulator.h>

namespace OSI {
namespace WSB {

//-- Dispatch
class ParallelDispatchQueue;
class SerialDispatchQueue;

//-- Defines
#define BROKERING_TREE_NS "urn:idyria:osi:wsf:wsb:brokeringtree"

/**
 * A Broker Tree is a tree describing the intermediaries that a message could go through.
 * It also contains the methods to execute a message, that means pass a message through the tree, in Up or Down direction
 */
class BrokeringTree : public Intermediary {

    protected:

        /** \defgroup Utilities Utilities */
        /**@{*/

        /// All Intermediaries with an id attribute are indexed in this map
        map<string,Intermediary*> intermediariesIndex;

        /**@}*/

        /** \defgroup Tree Tree variables */
        /**@{*/

        list<Intermediary*> roots;

        /**@}*/

        /** \defgroup Work Intermediary Work dispatch */
        /**@{*/

        /// A Parallel dispatch queue, created on down() start
        ParallelDispatchQueue * dispatchParallelQueue;

        /// A Serial dispatch queue , created on down() start
        SerialDispatchQueue * dispatchSerialQueue;

        /**@}*/

    public:
        BrokeringTree();
        virtual ~BrokeringTree();

        /**\defgroup LoadTree Load Tree
         * \brief Methods to read an XML Tree description into this Broker Tree
         */
        /**@{*/

        void readInFromString(string xml);

        /**@}*/

        /**
         *
         * @param context
         */
        virtual void down(MessageContext * context)
                throw (IntermediaryException);

        /**
         * Do like down, but create a reversed intermediary list, and execute reversed
         * Calls #up(MessageContext *,Intermediary*) with a NULL stop
         * @param context
         */
        virtual void up(MessageContext * context) throw (IntermediaryException);

        /**
         *
         * @param context
         * @param stop The intermediary at which to stop : Calling will start at the first parent of the given stop intermediary
         */
        virtual void up(MessageContext * context, Intermediary * stop)
                throw (IntermediaryException);


        /** \defgroup Utilities Utilities */
        /** @{ */


        /**
         * Returns the intermediary indexed under the provided id
         * @param id
         * @return A pointer to the intermediary, NULL if none registered
         */
        Intermediary * getIntermediaryFromId(string id);

        /**
         * Returns a DOMManipulator containing a document representing the actual brokering tree
         *
         * @warning The user should free the Manipulator
         * @return
         */
        DOMManipulator  toXML();


        /** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* BrokeringTree_H_ */
