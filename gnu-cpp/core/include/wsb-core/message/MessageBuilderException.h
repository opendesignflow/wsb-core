/*
 * MessageBuilderException.h
 *
 *  Created on: Nov 9, 2011
 *      Author: rleys
 */

#ifndef MESSAGEBUILDEREXCEPTION_H_
#define MESSAGEBUILDEREXCEPTION_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

namespace OSI {
namespace WSB {

class MessageBuilderException: public Exception {
public:
	MessageBuilderException();
	MessageBuilderException(string message);
	virtual ~MessageBuilderException() throw();




};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGEBUILDEREXCEPTION_H_ */
