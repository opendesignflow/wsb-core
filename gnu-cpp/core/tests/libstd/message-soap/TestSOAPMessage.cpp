
// Includes
//-----------------

//-- Std
#include <fstream>
#include <iostream>
using namespace std;

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>
#include <wsb-core/broker/IntermediariesFactory.h>


//-- SAML2
#include <wsb-libstd/message/soap/SOAPMessage.h>

//-- Gtest
#include <gtest/gtest.h>

using namespace OSI::WSB;





/**
 * Read Brokering tree, and checks index
 */
TEST(message_soap,empty_soap) {


    // Create SOAP Message
    //------------
    SOAPMessage soapMessage;


    // Get Back Header and Body to make sure XML is working
    //----------------
    //soapMessage.getHeader();
    //soapMessage.getBody();



}
