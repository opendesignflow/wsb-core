/*
 * LifecycleException.cpp
 *
 *  Created on: Nov 14, 2011
 *      Author: rleys
 */

// Includes
//--------------------

//-- Std
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>


#include "LifecycleException.h"

namespace OSI {
namespace WSB {

LifecycleException::LifecycleException() : Exception() {


}

LifecycleException::LifecycleException(string message) : Exception(message) {


}

LifecycleException::~LifecycleException() throw(){
	// TODO Auto-generated destructor stub
}

LifecycleException & LifecycleException::operator<<(const char* msg) {

	this->message.append(msg);

	return *this;
}

LifecycleException & LifecycleException::operator<<(string & msg) {

	this->message.append(msg);

	return *this;
}

LifecycleException & LifecycleException::operator<<(LIFECYCLE_STATE state) {

	stringstream str;

	switch( state )
   {
	  case LIFECYCLE_INITIAL: str << "INITIAL"; break;
	  case LIFECYCLE_STARTED: str << "STARTED"; break;
	  case LIFECYCLE_STOPPED: str << "STOPPED"; break;
   }

	//char str[50];
	//sprintf(str,"%id",state);
	this->message.append(str.str());

	return *this;
}

LifecycleException & LifecycleException::operator<<(int & msg) {

	char str[50];
	sprintf(str,"%id",msg);
	this->message.append(str);

	return *this;
}


std::stringstream& operator<<( std::stringstream& os, LIFECYCLE_STATE& state )
{
   switch( state )
   {
      case LIFECYCLE_INITIAL: os << "INITIAL"; break;
      case LIFECYCLE_STARTED: os << "STARTED"; break;
      case LIFECYCLE_STOPPED: os << "STOPPED"; break;
   }

   return os;
}


} /* namespace WSB */
} /* namespace OSI */
