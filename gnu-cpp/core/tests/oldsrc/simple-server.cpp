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

//-- Network
#include <common/Lifecycle.h>
#include <network/AbstractConnector.h>
#include <network/LoopbackConnector.h>
#include <network/NetworkLayer.h>


//-- WSB
#include <engine/WSEngine.h>

using namespace EUVNI::WSB;


// Prototypes
//-----------------
string readFile(const char * path);

int main() {


	cout << "Simple test server" << endl;



	// Register Intermediaries
	//--------------------------------
	HeaderInjectTimestampFactory * f = new HeaderInjectTimestampFactory();
	IntermediariesFactory::provideIntermediary("HeaderInjectTimestamp",f);


	// Prepare Engine
	//-------------------------
	EUVNI::WSB::WSEngine * engine = new EUVNI::WSB::WSEngine();


	// Network
	//-------------------------

	//-- Create Loopback connector for SOAP
    LoopbackConnector * connector = new LoopbackConnector();
    connector->setMessageType("soap");

    //---- Add A broker tree
    EUVNI::WSB::BrokeringTree * connectorBTree = new EUVNI::WSB::BrokeringTree();
    connectorBTree->readInFromString(readFile("messages/empty_connector_tree.xml"));
    connector->setBrokeringTree(connectorBTree);

	//-- Add  loopback connector to Network
	engine->getNetwork().addConnector(static_cast<AbstractConnector*>(connector));

	// Start
	//-------------------------
	engine->start();

	// Test Message interface
	//---------------------------------------

	//-- Inject messages
	connector->loopMessage(readFile("messages/empty.xml"));
	//connector->loopMessage(readFile("messages/wrong_syntax_empty.xml"));



	cout << "*I: Press any key to exit" << endl;
	char c;
	cin >> c;

	engine->stop();

	return 0;

}


string readFile(const char * path) {

	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());
	return str;

}



