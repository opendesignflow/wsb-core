/*
 * WSAddressing .h
 *
 *  Created on: TODO
 *  Author: rleys
 */

#ifndef WSADDRESSING_H_
#define WSADDRESSING_H_

// Include
//-----------------

//-- Broker
#include <wsb-core/broker/IntermediaryFactory.h>
#include <wsb-core/broker/Intermediary.h>

// Namespace
#define NS_WSA "http://www.w3.org/2005/08/addressing"

namespace OSI {
namespace WSB {

class WSAddressing: public Intermediary, public IntermediaryFactory {
    public:
        WSAddressing();
        virtual ~WSAddressing();

        /** \defgroup Factory Factory Methods */
        /** @{ */

        virtual string getSupportedIntermediaryName();

        virtual Intermediary * createIntermediary();

        /** @} */

        /** \defgroup Intermediary Intermediary Methods */
        /** @{ */

        virtual void down(MessageContext * context)
                throw (IntermediaryException);


        virtual void up(MessageContext * context) throw (IntermediaryException);

        /** @} */

};

} // EOF Namespace OSI
} // EOF Namespace WSB

#endif /* WSADDRESSING_H_ */

