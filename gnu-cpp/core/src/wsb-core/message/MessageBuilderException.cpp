/*
 * MessageBuilderException.cpp
 *
 *  Created on: Nov 9, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

#include "MessageBuilderException.h"

namespace OSI {
namespace WSB {

MessageBuilderException::MessageBuilderException() {
	// TODO Auto-generated constructor stub

}

MessageBuilderException::MessageBuilderException(string message) {
	this->message = message;
}

MessageBuilderException::~MessageBuilderException() throw() {
	// TODO Auto-generated destructor stub
}

} /* namespace WSB */
} /* namespace OSI */
