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

//-- Common
#include <common/Lifecycle.h>

//-- Broker
#include <broker/BrokeringTree.h>
#include <broker/IntermediariesFactory.h>
#include <broker/IntermediaryFactory.h>

//-- Standard
#include <standard/intermediary/HeaderInjectTimestampFactory.h>
#include <standard/intermediary/WSAddressingFactory.h>

#include <standard/connector/UnixSocketConnector.h>

//-- Message
#include <message/soap/SOAPMessage.h>

//-- Network
#include <network/NetworkLayer.h>
#include <network/AbstractConnector.h>
#include <network/LoopbackConnector.h>



//-- WSB
#include <engine/WSEngine.h>

using namespace EUVNI::WSB;


// Prototypes
//-----------------
string readFile(const char * path);

int main() {


	cout << "Simple test server" << endl;




	if(fork()==0) {



		// Client
		//--------------------------

		//-- Prepare Engine
		//-------------------------
		EUVNI::WSB::WSEngine * engine = new EUVNI::WSB::WSEngine();
		engine->init();

		//-- Register Intermediaries
		//--------------------------------
		HeaderInjectTimestampFactory * f = new HeaderInjectTimestampFactory();
		IntermediariesFactory::provideIntermediary("HeaderInjectTimestamp",f);
		IntermediariesFactory::provideIntermediary("WSAddressing",new WSAddressingFactory());

		//-- Network
		//------------------
		engine->getNetwork().addConnector(new UnixSocketConnector());

		//-- Brokering Tree
		//----------------------------
		engine->getBroker().readInFromFile("broker/req_client_broker.xml");

		//-- Start
		//-------------------------
		engine->start();


		//-- Send something
		//---------------------------

		//-- Choose Connector
		NetworkContext * networkContext = new NetworkContext("unix://wsb");

		//-- Prepare message content
		SOAPMessage * soap = new SOAPMessage();

		//-- Create
		MessageContext * message = new MessageContext(soap,networkContext);

		//-- Send
		engine->getBroker().send(message);


		sleep(2);

	} else {


		// SERVER
		//--------------------------

		//-- Register Intermediaries
		//--------------------------------
		/*HeaderInjectTimestampFactory * f = new HeaderInjectTimestampFactory();
		IntermediariesFactory::provideIntermediary("HeaderInjectTimestamp",f);


		//-- Prepare Engine
		//-------------------------
		EUVNI::WSB::WSEngine * engine = new EUVNI::WSB::WSEngine();

		//-- Try out Loopback Connector
		//-------------------------------------

		//-- Create Loopback connector for SOAP
		LoopbackConnector * connector = new LoopbackConnector();
		connector->setMessageType("soap");

		//-- Add A broker tree
		EUVNI::WSB::BrokeringTree * connectorBTree = new EUVNI::WSB::BrokeringTree();
		connectorBTree->readInFromString(readFile("messages/empty_connector_tree.xml"));
		connector->setBrokeringTree(connectorBTree);




		//-- Network
		//-------------------------

		//-- Add A loopback connector
		engine->getNetwork().addConnector(static_cast<AbstractConnector*>(connector));

		// Start
		//-------------------------
		engine->start();*/

	}

	return 0;

}


string readFile(const char * path) {

	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());
	return str;

}



