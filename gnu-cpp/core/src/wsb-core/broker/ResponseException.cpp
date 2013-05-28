/*
 * ResponseException.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

// Includes
//-----------------------

//-- Std
#include <string>
using namespace std;

#include "ResponseException.h"

namespace OSI {
namespace WSB {

ResponseException::ResponseException(Message * responseMessage) : Exception() {
    this->responseMessage = responseMessage;
}

ResponseException::ResponseException(Message * responseMessage,string message)  : Exception(message) {
    this->responseMessage = responseMessage;
}

ResponseException::ResponseException(Message * responseMessage,const char * message)  : Exception(message) {
    this->responseMessage = responseMessage;
}

ResponseException::ResponseException(const ResponseException& ex) {
    this->message = ex.message;
    this->responseMessage = ex.responseMessage;
}

ResponseException::~ResponseException() throw () {

}

/// Streams the char * into the actual message string
ResponseException & ResponseException::operator<<(const char* msg) {
    this->message.append(msg);

    return *this;
}

/// Streams into the actual message string
ResponseException & ResponseException::operator<<(long unsigned int msg){

    char str[50];
    sprintf(str,"%lu",msg);
    this->message.append(str);

    return *this;

}

/// Streams the string into the actual message string
ResponseException & ResponseException::operator<<(string & msg){
    this->message.append(msg);

    return *this;

}

Message * ResponseException::getResponseMessage() {
    return this->responseMessage;
}

} /* namespace WSB */
} /* namespace OSI */
