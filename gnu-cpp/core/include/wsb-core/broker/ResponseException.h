/*
 * ResponseException.h
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

#ifndef RESPONSEEXCEPTION_H_
#define RESPONSEEXCEPTION_H_

// Includes
//------------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>
#include <wsb-core/message/Message.h>

namespace OSI {
namespace WSB {

/** Thrown by an Intermediary to trigger a normal response
 *
 */
class ResponseException: public Exception {

    protected:

        Message * responseMessage;

    public:
        ResponseException(Message * responseMessage);
        ResponseException(Message * responseMessage, string message);
        ResponseException(Message * responseMessage, const char * message);
        ResponseException(const ResponseException& ex);
        virtual ~ResponseException() throw ();

        /// Streams into the actual message string
        ResponseException & operator <<(long unsigned int);

        /// Streams the char * into the actual message string
        ResponseException & operator <<(const char*);

        /// Streams the string into the actual message string
        ResponseException & operator <<(string &);

        Message * getResponseMessage();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* RESPONSEEXCEPTION_H_ */
