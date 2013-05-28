/**
 * RPCHandler.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef RPCHANDLER_H_
#define RPCHANDLER_H_

// Includes
//----------------

//-- Message
#include <wsb-core/message/MessageContext.h>
using namespace OSI::WSB;

namespace OSI {
namespace WSB {
namespace LIBREMOTE {

/**
 * This class is an actual RPC handler that is created and invoked by the RPCHandlerFactory class.
 *
 *  - The called method is handle()
 *  - Some utilities are provided for the handler to get some payload data prefetched
 *
 */
class RPCHandler {
    public:
        RPCHandler();
        virtual ~RPCHandler();


        /**
         * To be implemented to handle the message
         * @param msg
         */
        virtual Message * handle(MessageContext * msg) = 0;

};

} /* namespace LIBREMOTE */
} /* namespace WSB */
} /* namespace OSI */
#endif /* RPCHANDLER_H_ */
