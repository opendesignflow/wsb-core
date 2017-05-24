/**
 * TestTCPMessageStream.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------


//-- Message
#include <message/MessageContext.h>

//-- SOAP
#include <wsb-libstd/message/soap/SOAPMessageFactory.h>


//-- Connector
#include <wsb-libstd/connector/tcp/MessageStreamTCPConnector.h>

//-- Gtest
#include <gtest/gtest.h>

using namespace OSI::WSB;

class MessageStreamTCPConnectorTest : public ::testing::Test {

    protected:

        /// Server Connector
        OSI::WSB::MessageStreamTCPConnector * server;

        /// Client Connector
        OSI::WSB::MessageStreamTCPConnector * client;

        virtual void SetUp() {

            // SOAP Message type
            //MessageFactory::getI

            // Server
            server = new OSI::WSB::MessageStreamTCPConnector();
            server->setMessageType("soap");

            server->init();
            server->start();
            server->joinStarted();


            // Client
            client = new OSI::WSB::MessageStreamTCPConnector();
            client->init();
            client->setDirection(AbstractConnector::CLIENT);
        }

        virtual void TearDown() {
            server->stop();
            server->finish();
            delete server;

            client->finish();
            delete client;
        }

    public:

        virtual ~MessageStreamTCPConnectorTest() {

        }

};

TEST_F(MessageStreamTCPConnectorTest, start_close_server) {

    server->stop();

    server->join();

    server->finish();



}

TEST_F(MessageStreamTCPConnectorTest, one_message) {

    // Prepare Network context
    //------------
    NetworkContext networkCtx("mstream://127.0.0.1:8087");


    // Prepare message
    //-------------------
    SOAPMessageFactory soapFactory;
    SOAPMessage * soapMessage = soapFactory.build();

    MessageContext messageContext(soapMessage,&networkCtx);


    // Send
    //------------
    this->client->send(&messageContext);


    // Wait 1ms for receive on server side
    //------------
    MessageContext * serverReceivedMsg = this->server->poll(1 * 1000 * 1000 * 1000);

    ASSERT_TRUE(serverReceivedMsg!=NULL);


    delete soapMessage;

}


TEST_F(MessageStreamTCPConnectorTest, multiple_messages) {


    // Test Data
    //------------------
    int messagesCount = 10;

    // Prepare Network context
    //------------
    NetworkContext networkCtx("mstream://127.0.0.1:8087");

    for (int i = 0 ; i < messagesCount ; i ++) {

        // Prepare message
        //-------------------
        SOAPMessageFactory soapFactory;
        SOAPMessage * soapMessage = soapFactory.build();

        MessageContext messageContext(soapMessage,&networkCtx);


        // Send
        //------------
        this->client->send(&messageContext);

        delete soapMessage;
    }





    // Wait 1s for receive on server side
    //------------
    for (int i = 0 ; i < messagesCount ; i ++) {

        MessageContext * serverReceivedMsg = this->server->poll(1 * 1000 * 1000 * 1000);

        ASSERT_TRUE(serverReceivedMsg!=NULL);
    }





}

