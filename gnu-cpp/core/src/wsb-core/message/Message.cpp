/*
 * Message.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//---------------------

//-- Std
#include <string>
using namespace std;


//-- Common
#include <wsb-core/common/Logging.h>

#include "Message.h"

namespace OSI {
namespace WSB {

Message::Message() {
	this->base = NULL;

}

Message::~Message() {

}

string Message::getType() {
	return this->messageType;
}

string Message::getQualifier() {
    return this->messageQualifier;
}

void Message::setQualifier(string qualifier) {
    this->messageQualifier = qualifier;
}


string Message::toString() {
	return string();
}



} /* namespace WSB */
} /* namespace OSI */
