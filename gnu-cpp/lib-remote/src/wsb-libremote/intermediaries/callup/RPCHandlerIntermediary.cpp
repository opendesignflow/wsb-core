/**
 * RPCHandlerIntermediary.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Common
#include <wsb-core/common/Logging.h>

//-- RPC Handler
#include <wsb-libremote/intermediaries/callup/RPCHandlerFactory.h>

#include "RPCHandlerIntermediary.h"

namespace OSI {
namespace WSB {
namespace LIBREMOTE {

RPCHandlerIntermediary::RPCHandlerIntermediary() {

}

RPCHandlerIntermediary::~RPCHandlerIntermediary() {
}



void RPCHandlerIntermediary::registerHandlerFactory(string messageQualifier,RPCHandlerFactory * factory) {

    this->handlerFactories[messageQualifier] = factory;

}

string RPCHandlerIntermediary::getSupportedIntermediaryName() {

    return "rpc-handler";

}

Intermediary * RPCHandlerIntermediary::createIntermediary() {

    return new RPCHandlerIntermediary();

}

void RPCHandlerIntermediary::down(MessageContext * context)
        throw (IntermediaryException,ResponseException) {


    // Get Qualifier
    //--------------------
    string qualifier = context->getMessage()->getQualifier();

    // No Qualifier -> fail
    //-----------------
    if (qualifier.length()==0) {

        stringstream ss;
        ss << "The input message did not have a qualifier. This is required to find a handler factory";

        Logging::getLogger("wsb.libremote.intermediary.rpchandler")->errorStream() << ss.str();

        throw ss.str();

    }
    // No Matching Factory -> fail
    //-----------------
    else if (this->handlerFactories.count(qualifier)==0) {

        stringstream ss;
        ss << "The input qualifier has no registered Factory. This is required to handle the message";

        Logging::getLogger("wsb.libremote.intermediary.rpchandler")->errorStream() << ss.str();

        throw ss.str();

    }

    // There is a Factory -> Proceed
    //--------------------
    RPCHandlerFactory * factory = this->handlerFactories[qualifier];

    //-- Instanciate handler
    RPCHandler *  handler = factory->newInstance(context);

    Message * response = handler->handle(context);


    // If there is a response -> trigger
    //-----------------
    if (response != NULL) {
        throw ResponseException(response);
    }

}

void RPCHandlerIntermediary::up(MessageContext * context)
        throw (IntermediaryException) {



}


} /* namespace LIBREMOTE */
} /* namespace WSB */
} /* namespace OSI */
