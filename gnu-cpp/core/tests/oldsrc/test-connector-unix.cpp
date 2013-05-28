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

		sleep(2);

		//-- Create connector
		UnixSocketConnector * usock = new UnixSocketConnector();

		//-- Create Message Context
		SOAPMessage * soap = new SOAPMessage();
		MessageContext * ctx  = new MessageContext(soap);

		//-- Send
		usock->send(ctx);


	} else {


		// SERVER
		//--------------------------


		//-- Create connector
		UnixSocketConnector * usock = new UnixSocketConnector();
		usock->init();
		usock->start();

		cout << "*I: Press any key to exit" << endl;
		char c;
		cin >> c;


		usock->stop();


	}

	return 0;

}


string readFile(const char * path) {

	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
	                 std::istreambuf_iterator<char>());
	return str;

}



