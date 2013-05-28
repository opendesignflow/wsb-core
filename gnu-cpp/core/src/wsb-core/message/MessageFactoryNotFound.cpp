/*
 * MessageFactoryNotFound.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

#include "MessageFactoryNotFound.h"

namespace OSI {
namespace WSB {

MessageFactoryNotFound::MessageFactoryNotFound() {
	// TODO Auto-generated constructor stub

}

MessageFactoryNotFound::MessageFactoryNotFound(string message) : Exception(message) {

}

MessageFactoryNotFound::~MessageFactoryNotFound() throw() {
	// TODO Auto-generated destructor stub
}

} /* namespace WSB */
} /* namespace OSI */
