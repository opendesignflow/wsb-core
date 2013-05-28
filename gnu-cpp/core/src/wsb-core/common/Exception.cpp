/*
 * Exception.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std
#include <exception>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;


#include "Exception.h"

namespace OSI {
namespace WSB {

Exception::Exception() {
    this->message = "";
}

Exception::Exception(string message) {
	this->message = message;
}

Exception::~Exception() throw() {

}

void Exception::setMessage(string message) {
	this->message = message;
}


Exception & Exception::operator<<(const char* msg) {

	this->message.append(msg);

	return *this;
}

Exception & Exception::operator<<(long unsigned int i) {

	char str[50];
	sprintf(str,"%lid",i);
	this->message.append(str);


	return *this;
}

Exception & Exception::operator<<(string & msg) {

	this->message.append(msg);

	return *this;
}

const char * Exception::what() const throw() {
	return this->message.c_str();
}



} /* namespace WSB */
} /* namespace OSI */
