/*
 * Intermediary.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef INTERMEDIARY_H_
#define INTERMEDIARY_H_

// Includes
//------------------

//-- Std
#include <list>
using namespace std;

//-- Broker
#include <wsb-core/broker/IntermediaryException.h>
#include <wsb-core/broker/ResponseException.h>

//-- Message
#include <wsb-core/message/MessageContext.h>

namespace OSI {
namespace WSB {

/**
 * Represents an Intermediary for a BrokeringTree
 */
class Intermediary {

    protected:

        /** \defgroup Tree Tree Definition for this intermediary */
        /**@{*/

        /// The children of this intermdiary
        list<Intermediary*> children;


        /// The ID set on the intermediary
        string id;

        /// The name set on the intermediary
        string name;

        /// The filter pattern to validate an intermediary depending on message qualifier
        regex_t * filter;



        /**@}*/

        /** \defgroup Management Management */
        /** \brief Variables/Methods used to define how this object should be managed*/
        /**@{*/



        /// The intermediary will be cleaned by brokering tree
        /// If false, the user is responsible for correct cleaning
        bool managed;

        /// This mutex is used to thread sync methods of this class
        pthread_mutex_t thread_sync;

        /**@}*/

    public:
        Intermediary();
        virtual ~Intermediary();

        /** \addtogroup Management */
        /**@{*/



        /**
         *  If false, will be memory managed by the parent.
         *  This Intermediary won't destroy its children that are defined managed
         * @return
         */
        bool isManaged();

        void setManaged(bool);

        /** @} */

        /** \addtogroup Tree */
        /**@{ */

        /**
         * Return the filter value for tree switching
         * @return the filter, or a NULL pointer if undefined, which is equivalent to wildcard
         */
        regex_t * getFilter();

        /**
         * Define the filter on this Intermediary
         * The string is compiled to a regex_t* type
         * @param filter
         * @throw IntermediaryException if the filter doesn't compile to a regular expression
         */
        void setFilter(string filter) throw (IntermediaryException);

        /**
         * Sets the ID of this intermediary
         *
         * This ID is used by brokering tree to index the Intermediary instances, and allow fast access.
         * If this value is changed at runtime, you have to ask the brokering tree for reindexing
         *
         * @param id an empty string unsets the id
         */
        void setId(string id);

        /**
         * Returns the id of this intermediray
         * @return an empty string if not defined
         */
        string getId();

        /**
         * Sets the name of this intermediary
         *
         *
         * @param name an empty string unsets the name
         */
        void setName(string name);

        /**
         * Returns the name of this intermediary
         * @return an empty string if not defined
         */
        string getName();

        /**
         * Add an Intermediary as Child to this one
         * If the intermediary is not managed, this Intermediary will manage it, which means delete it when it will be itself deleted
         * @note Thread safe
         * @param
         */
        void addChild(Intermediary *);

        /**
         * Returns the list of Intermediaries children
         * @return A reference to the list
         */
        list<Intermediary*> & getChildren();

        /** @} */

        /** \defgroup Message Processing */
        /** @{ */

        /**
         * Down is called upon reception of a message
         * if something wrong happens, Throw an Exception
         * @param context
         */
        virtual void down(MessageContext * context)
                throw (IntermediaryException,ResponseException) = 0;

        /**
         * Up is called upon sending of a message
         * if something wrong happens, Throw an Exception
         * @param context
         */
        virtual void up(MessageContext * context) throw (IntermediaryException) = 0;

        /** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* INTERMEDIARY_H_ */
