/*
 *TCLInvokeFactory.cpp
 *
 *  Created on: TODO
 *  Author: rleys
 */

// Includes
//-------------

//-- Broker
#include <broker/IntermediaryFactory.h>

#include "TCLInvoke.h"
#include "TCLInvokeFactory.h"


namespace EUVNI {
namespace WSB {
namespace TCL {


TCLInvokeFactory::TCLInvokeFactory() {

	//-- Defaults
	this->basePath = "./";

}

TCLInvokeFactory::~TCLInvokeFactory() {

}

void TCLInvokeFactory::setBasePath(string basePath) {
	this->basePath = basePath;

}
string TCLInvokeFactory::getBasePath() {
	return this->basePath;
}

// Creation Method
TCLInvoke * TCLInvokeFactory::createIntermediary() {

	return new TCLInvoke(this->basePath);

}


} // EOF Namespace EUVNI
} // EOF Namespace WSB
} // EOF Namespace TCL

