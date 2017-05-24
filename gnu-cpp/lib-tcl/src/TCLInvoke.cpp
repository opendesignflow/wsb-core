/*
 * TCLInvoke.cpp
 *
 *  Created on: TODO
 *  Author: rleys
 */

// Includes
//----------------

//-- Std
#include <iostream>
#include <fstream>
using namespace std;

//-- Broker
#include <broker/Intermediary.h>

//-- Logging
#include <common/Logging.h>

//-- Message
#include <message/soap/SOAPMessage.h>

//-- TCL
#include <tcl/tcl.h>

#include "TCLInvoke.h"

namespace EUVNI {
namespace WSB {
namespace TCL {


TCLInvoke::TCLInvoke(string basePath) {

	//-- Init base path
	this->basePath = basePath;

}

TCLInvoke::~TCLInvoke() {

}

/// To be Implemented
void TCLInvoke::down(MessageContext * context) throw (IntermediaryException) {



	//-- Report We are here:
	Logging::getLogger("wsb.tcl.invoke")->infoStream() << "Invoke TCL script";

	// Find the script path
	//--------------------------
	SOAPMessage * soap = dynamic_cast<SOAPMessage*>(context->getMessage());
	soap->addXPathNamespace("wsbtcl",NS_TCL);

	DOMElement * tclinvoke = soap->getHeaderElement("wsbtcl:InvokeTCL");

	if (tclinvoke==NULL || !tclinvoke->hasAttribute(X("script"))) {
		Logging::getLogger("wsb.tcl.invoke")->warnStream() << "TCL Invoke header not found or wrong";
		IntermediaryException ex("TCL Invoke header not found or wrong");
		throw  ex;
	}

	// Check we have the script under the base path
	//-----------------------------------
	string scriptPath = this->basePath+"/"+X2S(tclinvoke->getAttribute(X("script")));

	std::ifstream t;
	t.open(scriptPath.c_str(),ifstream::in);
	if (t.fail()) {
		Logging::getLogger("wsb.tcl.invoke")->warnStream() << "TCL Invoke script not found: " << scriptPath;
		IntermediaryException ex;
		ex << "TCL Invoke script not found: "  << scriptPath;
        throw  ex;
	}

	//-- Get content
	std::string scriptContent((std::istreambuf_iterator<char>(t)),
					 std::istreambuf_iterator<char>());

	// Evaluate
	//-----------------------------------

	//-- Create an Interpreter
	Tcl_Interp * interpreter = Tcl_CreateInterp();
	Tcl_Eval(interpreter, scriptContent.c_str());



}

/// To be Implemented
void TCLInvoke::up(MessageContext * context) throw (IntermediaryException) {

}



} // EOF Namespace EUVNI
} // EOF Namespace WSB
} // EOF Namespace TCL

