/**
 * RPCHandlerIntermediary.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef RPCHANDLERINTERMEDIARY_H_
#define RPCHANDLERINTERMEDIARY_H_

// Includes
//-----------------

//-- Std
#include <string>
#include <map>
using namespace std;

//-- Core
#include <wsb-core/broker/IntermediaryFactory.h>
#include <wsb-core/broker/Intermediary.h>
using namespace OSI::WSB;

//-- RPCHandler
#include <wsb-libremote/intermediaries/callup/RPCHandlerFactory.h>

namespace OSI {
namespace WSB {
namespace LIBREMOTE {

/**
 *
 * This Intermediary uses an internal mapping between action strings and RPCHandlerFactory instances
 * to dynamically route incoming messages to a registered handler
 *
 *
 *
 */
class RPCHandlerIntermediary : public Intermediary ,  public IntermediaryFactory {

    protected:

        /**
         * The Handler factories map associates message qualifiers  handler factories to access user registered handlered for messages
         */
        map<string,RPCHandlerFactory*> handlerFactories;




    public:
        RPCHandlerIntermediary();
        virtual ~RPCHandlerIntermediary();

        /** \defgroup Message Message Processing */
        /** @{ */

        /**
         * Registers a handler factory for a given messageQualifier
         * If a Factory already exists, the pointer is replaced
         * @param messageQualifier
         * @param factory The pointer to the factory to register. NULL deregisters the factory.
         * @warning The Factory pointer is managed by the user, not this class instance
         */
        void registerHandlerFactory(string messageQualifier,RPCHandlerFactory * factory);


        /** @} */


        /** \defgroup Factory Factory Methods */
        /** @{ */

        virtual string getSupportedIntermediaryName();

        virtual Intermediary * createIntermediary();

        /** @} */

        /** \defgroup Intermediary Intermediary Methods */
        /** @{ */

        /**
         *
         * Throws an IntermediaryException if the qualifier of the message woin't match any Handler Factory
         *
         * @param context
         */
        virtual void down(MessageContext * context) throw (IntermediaryException,ResponseException);


        /**
         * Nothing to do
         * @param context
         */
        virtual void up(MessageContext * context) throw (IntermediaryException);

        /** }@ */

};

} /* namespace LIBREMOTE */
} /* namespace WSB */
} /* namespace OSI */
#endif /* RPCHANDLERINTERMEDIARY_H_ */
