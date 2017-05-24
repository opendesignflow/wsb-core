/**
 * RPCHandlerFactory.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef RPCHANDLERFACTORY_H_
#define RPCHANDLERFACTORY_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Message
#include <wsb-core/message/MessageContext.h>
using namespace OSI::WSB;

#include <wsb-libremote/intermediaries/callup/RPCHandler.h>

namespace OSI {
namespace WSB {
namespace LIBREMOTE {


class RPCHandlerFactory {

    protected:

        /// The action string to be associated with this factory
        string action;

        /// The eventual payload name for this factory. Used for validation
        /// Not used if empty
        string payloadName;

        /// The eventual payload namespace for this factory. Used for validation
        /// Not used if empty
        string payloadNS;

    public:
        RPCHandlerFactory();
        virtual ~RPCHandlerFactory();

        /**
         * This pure virtual method is to be implemented to create a new Instance
         * of an RPC handler
         *
         * @param message The message context for which to create a Handler
         */
        virtual RPCHandler* newInstance(MessageContext * message) = 0;


        /** Returns the action string this factory is associated to
         *
         * @return
         */
        string getAction() const;

        /** Sets the action string this factory is associated to
         *
         * @param action
         */
        void setAction(string action);

        string getPayloadName() const;
        void setPayloadName(string payloadName);

        string getPayloadNs() const;
        void setPayloadNs(string payloadNs);


}
;
} /* namespace LIBREMOTE */
} /* namespace WSB */
} /* namespace OSI */
#endif /* RPCHANDLERFACTORY_H_ */
