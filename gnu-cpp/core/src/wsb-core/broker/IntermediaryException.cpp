/*
 * IntermediaryException.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

// Includes
//-----------------------

//-- Std
#include <string>
using namespace std;

#include "IntermediaryException.h"

namespace OSI {
namespace WSB {

IntermediaryException::IntermediaryException() : Exception() {

}

IntermediaryException::IntermediaryException(string message)  : Exception(message) {

}

IntermediaryException::IntermediaryException(const char * message)  : Exception(message) {

}

IntermediaryException::IntermediaryException(IntermediaryException & ex) {
    this->message = ex.message;
}

IntermediaryException::~IntermediaryException() throw () {

}

/// Streams the char * into the actual message string
IntermediaryException & IntermediaryException::operator<<(const char* msg) {
    this->message.append(msg);

    return *this;
}

/// Streams into the actual message string
IntermediaryException & IntermediaryException::operator<<(long unsigned int msg){

    char str[50];
    sprintf(str,"%lu",msg);
    this->message.append(str);

    return *this;

}

/// Streams the string into the actual message string
IntermediaryException & IntermediaryException::operator<<(string & msg){
    this->message.append(msg);

    return *this;

}

} /* namespace WSB */
} /* namespace OSI */
