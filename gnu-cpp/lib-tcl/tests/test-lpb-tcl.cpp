/*
 * simple-server.cpp
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

// Includes
//---------------------

//-- Std
#include <iostream>
#include <istream>
#include <fstream>
using namespace std;


//-- Broker
#include <broker/BrokeringTree.h>
#include <broker/IntermediariesFactory.h>
#include <broker/IntermediaryFactory.h>

//-- Standard
#include <standard/intermediary/HeaderInjectTimestampFactory.h>

//-- Message
#include <message/soap/SOAPMessage.h>

//-- Network
#include <common/Lifecycle.h>
#include <network/AbstractConnector.h>
#include <network/LoopbackConnector.h>
#include <network/NetworkLayer.h>

//-- WSB
#include <engine/WSEngine.h>
using namespace EUVNI::WSB;

//-- TCL
#include <TCLInvokeFactory.h>
using namespace EUVNI::WSB::TCL;

// Prototypes
//-----------------
string readFile(const char * path);

/**
 * This test to have a loopback server, that executes scripts passed in wsa:Action
 * @return
 */
int main() {


	cout << "TCL Invoker Loopback tester" << endl;



	// Register Intermediaries
	//--------------------------------
	HeaderInjectTimestampFactory * f = new HeaderInjectTimestampFactory();
	IntermediariesFactory::provideIntermediary("HeaderInjectTimestamp",f);

	TCLInvokeFactory * tclInvokeF = new TCLInvokeFactory();
	tclInvokeF->setBasePath("tests/tcl");
	IntermediariesFactory::provideIntermediary("TCLInvoke",tclInvokeF);




	// Prepare Engine
	//-------------------------
	EUVNI::WSB::WSEngine * engine = new EUVNI::WSB::WSEngine();
	engine->init();

	// Prepare Loopback Connector
	//-------------------------------------

	//-- Create Loopback connector for SOAP
	LoopbackConnector * connector = new LoopbackConnector();
	connector->setMessageType("soap");
	engine->getNetwork().addConnector(static_cast<AbstractConnector*>(connector));

	// Brokering Tree for TCL
	//-------------------------
	engine->getBroker().readInFromString(readFile("tests/broker/tcl_broker.xml"));

	// Start
	//-------------------------

	engine->start();

	// Test Message interface
	//---------------------------------------
	while(1) {

		string input;
		cout << "*I: Enter a TCL script name, or 'q' to stop" << endl;
		cin >> input;

		//-- Quit ?
		if (input=="q")
			break;

		//-- Prepare SOAP Message
		//---------------

		SOAPMessage * soap = new SOAPMessage();
		// <wsbtcl:InvokeTCL xmlns:wsbtcl="euvni:wsb:tcl" script="scriptname.tcl" />
		DOMElement * elt = soap->addHeader("euvni:wsb:tcl","InvokeTCL");
		elt->setAttribute(X("script"),X(input.c_str()));

		//-- Add Action

		//-- Inject
		connector->loopMessage(soap->toString());

		delete soap;

	}

	//-- Inject messages
	//connector->loopMessage(readFile("messages/empty.xml"));
	//connector->loopMessage(readFile("messages/wrong_syntax_empty.xml"));

	engine->stop();
	engine->finish();

	return 0;

}


string readFile(const char * path) {

	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());
	return str;

}



