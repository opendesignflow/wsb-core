/**
 * RPCHandlerFactory.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#include "RPCHandlerFactory.h"

namespace OSI {
namespace WSB {
namespace LIBREMOTE {



RPCHandlerFactory::RPCHandlerFactory() {
    // TODO Auto-generated constructor stub

}

RPCHandlerFactory::~RPCHandlerFactory() {
    // TODO Auto-generated destructor stub
}

string RPCHandlerFactory::getAction() const {
    return action;
}

void RPCHandlerFactory::setAction(string action) {
    this->action = action;
}

string RPCHandlerFactory::getPayloadName() const {
    return payloadName;
}

void RPCHandlerFactory::setPayloadName(string payloadName) {
    this->payloadName = payloadName;
}

string RPCHandlerFactory::getPayloadNs() const {
    return payloadNS;
}

void RPCHandlerFactory::setPayloadNs(string payloadNs) {
    payloadNS = payloadNs;
}



} /* namespace LIBREMOTE */
} /* namespace WSB */
}


 /* namespace OSI */
