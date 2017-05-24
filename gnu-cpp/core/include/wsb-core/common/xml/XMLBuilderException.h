/*
 * XMLBuilderException.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef XMLBUILDEREXCEPTION_H_
#define XMLBUILDEREXCEPTION_H_

// Include
//------------------------

//-- Common
#include <wsb-core/common/Exception.h>

namespace OSI {
namespace WSB {

class XMLBuilderException : public Exception {
public:
	XMLBuilderException();
	virtual ~XMLBuilderException() throw();

	/// Streams the char * into the actual message string
	Exception & operator<<(const char*str) {
		return Exception::operator<<(str);
	}

	/// Streams the string into the actual message string
	Exception & operator<<(string & str){
		return Exception::operator<<(str);
	}

	/// Streams into the actual message string
	Exception & operator<<(long unsigned int i){
		return Exception::operator<<(i);
	}




};

} /* namespace WSB */
} /* namespace OSI */
#endif /* XMLBUILDEREXCEPTION_H_ */
