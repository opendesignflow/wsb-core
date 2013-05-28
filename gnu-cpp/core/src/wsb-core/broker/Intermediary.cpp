/*
 * Intermediary.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */


// Includes
//------------------

//-- Std
#include <string>
#include <sstream>
using namespace std;

//-- Regexp
#include <re_comp.h>
#include <regex.h>

//-- Sync
#include <wsb-core/common/dispatch/MutexLocker.h>

//-- Message
#include <wsb-core/message/MessageContext.h>


#include "Intermediary.h"

namespace OSI {
namespace WSB {

Intermediary::Intermediary() {

	//-- Defaults
	this->managed 		= false;
	this->thread_sync 	= PTHREAD_MUTEX_INITIALIZER;
	this->filter = NULL;

}

Intermediary::~Intermediary() {
	// TODO Auto-generated destructor stub
}




bool Intermediary::isManaged()  {
	return this->managed;
}

void Intermediary::setManaged(bool managed) {
	this->managed = managed;
}

regex_t* Intermediary::getFilter() {
    return this->filter;
}


void Intermediary::setFilter(string filter) throw (IntermediaryException) {

    //-- Allocate filter
    this->filter = (regex_t*) calloc(1,sizeof(regex_t));

    //-- Compile Regexp
    int errcode = regcomp(this->filter,filter.c_str(),REG_EXTENDED);

    //-- There has been an error
    if (errcode!=0) {

        //-- Prepare Message
        stringstream ss;
        size_t length = regerror (errcode, this->filter, NULL, 0);
        char *buffer = (char*) calloc (length,sizeof(char));
        (void) regerror (errcode,this->filter, buffer, length);

        ss << "Error while compiling expression "<< filter << ", with message: " << buffer;


        //-- Deallocate filter
        free(this->filter);
        this->filter = NULL;

        //-- Throw Exception
        throw ss.str();


    }

}



void Intermediary::setId(string id) {

    this->id = id;

}


string Intermediary::getId() {

    return this->id;

}


void Intermediary::setName(string name) {
    this->name = name;
}


string Intermediary::getName() {
    return this->name;
}

void Intermediary::addChild(Intermediary * intermediary) {

	//-- Sync
	MutexLocker lock(&(this->thread_sync));

	//-- Register
	this->children.push_back(intermediary);

}

list<Intermediary*>& Intermediary::getChildren() {
	return this->children;
}



} /* namespace WSB */
} /* namespace OSI */
