/*
 * HeaderInjectTimestamp.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

// Includes
//--------------------------

//-- Std
#include <ctime>
#include <sstream>
using namespace std;

//-- Broker
#include <wsb-core/broker/IntermediaryException.h>
#include <wsb-core/broker/Intermediary.h>

//-- Message
#include <wsb-libstd/message/soap/SOAPMessage.h>

//-- Common
#include <wsb-core/common/Logging.h>

#include "HeaderInjectTimestamp.h"

namespace OSI {
namespace WSB {

HeaderInjectTimestamp::HeaderInjectTimestamp() {
	// TODO Auto-generated constructor stub

}

HeaderInjectTimestamp::~HeaderInjectTimestamp() {
	// TODO Auto-generated destructor stub
}

void HeaderInjectTimestamp::down(MessageContext * context) throw (IntermediaryException) {


	Logging::getLogger("standart.inter.header.timestamp")->info("Will be injecting timestamp");

	//-- Get TS
	time_t ts = time(NULL);

	//-- Inject
	//-- Work only with SOAP message
	if (context->getMessage()->getType()=="soap") {

		SOAPMessage * soap = static_cast<SOAPMessage*>(context->getMessage());
		stringstream ss;
		ss<<ts;
		soap->addHeader("OSI:wsb:std","Timestamp",ss.str());


	}


	//context->addMessageHeader("standard","Timestamp");

	Logging::getLogger("standart.inter.header.timestamp")->info("Will be injecting timestamp: %li",ts);


	//-- Stream out message
	Logging::getLogger("standart.inter.header.timestamp")->infoStream() << "Message is now" << context->getMessage()->toString();



}


void HeaderInjectTimestamp::up(MessageContext * context) throw (IntermediaryException) {

	Logging::getLogger("standart.inter.header.timestamp")->info("Will be injecting timestamp");

	//-- Get TS
	time_t ts = time(NULL);

	//-- Inject
	//-- Work only with SOAP message
	if (context->getMessage()->getType()=="soap") {

		SOAPMessage * soap = static_cast<SOAPMessage*>(context->getMessage());
		stringstream ss;
		ss<<ts;
		soap->addHeader("OSI:wsb:std","Timestamp",ss.str());


	} else {
		Logging::getLogger("standart.inter.header.timestamp")->info("Will be injecting timestamp: %li",ts);
	}


	//context->addMessageHeader("standard","Timestamp");

	Logging::getLogger("standart.inter.header.timestamp")->info("Will be injecting timestamp: %li",ts);


	//-- Stream out message
	Logging::getLogger("standart.inter.header.timestamp")->infoStream() << "Message is now" << context->getMessage()->toString();

}

} /* namespace WSB */
} /* namespace OSI */
