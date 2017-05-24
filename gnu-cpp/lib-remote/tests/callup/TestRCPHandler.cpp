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

//-- Std
#include <semaphore.h>
#include <cmath>

//-- Wsb for test
#include <wsb-core/engine/WSEngine.h>
#include <wsb-core/broker/BrokeringTree.h>

#include <WSEngineWithLoopback.hpp>

//-- Standard
#include <wsb-libstd/intermediary/WSAddressing.h>
#include <wsb-libstd/message/soap/SOAPMessageFactory.h>

//-- RPC Intermediary
#include <wsb-libremote/intermediaries/callup/RPCHandlerIntermediary.h>

//-- Gtest
#include <gtest/gtest.h>

using namespace OSI::WSB;
using namespace OSI::WSB::LIBREMOTE;

/**
 * Add Support for WS-Adressing wsa:Action to extract the qualifier
 */
class TestRCPHandler: public WSEngineWithLoopback {

    protected:

        WSAddressing addressing;

    public:

        TestRCPHandler() :
                WSEngineWithLoopback() {

        }

        virtual ~TestRCPHandler() {

        }

    protected:

        virtual void SetUp() {

            WSEngineWithLoopback::SetUp();

            // Enrich Brokering Tree
            //-------------------------------
            BrokeringTree * brokeringTree = new BrokeringTree();
            this->getBroker().setBrokeringTree(brokeringTree);

            //-- Add WS-Adressing for qualifier extraction
            this->getBroker().getBrokeringTree()->addChild(&addressing);

            //-- Add Our action detector

        }
};

/**
 * This Signaling handler releases grants in a Semaphore when reached, to allow test methods to wait
 */
class SignalingHandler : public RPCHandlerFactory, public RPCHandler {

    protected:

        /// Signaling Semaphore
        sem_t signalSemaphore;

    public:
        SignalingHandler() {

            //-- Init Sync
            sem_init(&(this->signalSemaphore), 0, 0);

        }
        virtual ~SignalingHandler() {

        }

        virtual RPCHandler * newInstance(MessageContext * context) {
            return this;
        }

        virtual Message * handle(MessageContext * context) {


            Logging::getLogger("signalinghandler")->infoStream() << "Signaling in handler";

            //-- Post semaphore
            sem_post(&(this->signalSemaphore));

            return NULL;
        }

        /**
         * Waits for the semaphore to be released
         * @return true on success, false on timeout
         */
        bool join(long int waitns) {

            //-- Wait on semaphore
            int res = -1;
            if (waitns == 0 ) {
                res = sem_wait(&(this->signalSemaphore));
            } else {

                //struct timespec waitTime;
                struct timespec tm;
                clock_gettime(CLOCK_REALTIME, &tm);

                //tm.tv_nsec += waitTime.tv_nsec;
                //tm.tv_sec +=  waitTime.tv_sec;

                tm.tv_sec += floor((waitns/(1000*1000*1000)));
                tm.tv_nsec += (waitns%(1000*1000*1000));

                res = sem_timedwait(&(this->signalSemaphore),&tm);
            }

            if (res!=0) {
                //-- An error occured, -> return NULL
                return false;
            }
            return true;


        }

        /**
         * Return number of grants in the semaphore
         *
         * @return <0 if an error occured, otherwise the number
         */
        int getGrantsCount() {

            int res;
            if(sem_getvalue(&(this->signalSemaphore), &res)==-1) {
                return -1;
            }

            return res;
        }




};

//class SignalingIntermediray

// Sync Intermediary
//-

TEST_F(TestRCPHandler, fail_witness) {

    string qualifier = "tests.rpchandler.one_action";

    // Add A Single RPCIntermediary
    //----------------------
    RPCHandlerIntermediary *  rpcIntermediary =  new RPCHandlerIntermediary();
    this->addressing.addChild(rpcIntermediary);

    //-- Register Signaling RPC method
    SignalingHandler * handler = new SignalingHandler();
    rpcIntermediary->registerHandlerFactory(qualifier,handler);

    //-- Prepare a Simple Action message
    //----------------
    SOAPMessageFactory soapFactory;
    SOAPMessage * soapMessage = soapFactory.build();

    //-- Add WS-Adressing
    soapMessage->addXPathNamespace("wsa",
            "http://www.w3.org/2005/08/addressing");
    soapMessage->addHeader("http://www.w3.org/2005/08/addressing", "wsa:Action",
            qualifier);


    // Loopback message
    //----------------------------
    this->sendMessage(soapMessage->toString());

    // Wait For reception (1 ns)
    //-----------------------------
    ASSERT_FALSE(handler->join(1));



    //SOAPMessage msg

}

TEST_F(TestRCPHandler, one_action) {

    string qualifier = "tests.rpchandler.one_action";

    // Add A Single RPCIntermediary
    //----------------------
    RPCHandlerIntermediary *  rpcIntermediary =  new RPCHandlerIntermediary();
    this->addressing.addChild(rpcIntermediary);

    //-- Register Signaling RPC method
    SignalingHandler * handler = new SignalingHandler();
    rpcIntermediary->registerHandlerFactory(qualifier,handler);

    //-- Prepare a Simple Action message
    //----------------
    SOAPMessageFactory soapFactory;
    SOAPMessage * soapMessage = soapFactory.build();

    //-- Add WS-Adressing
    soapMessage->addXPathNamespace("wsa",
            "http://www.w3.org/2005/08/addressing");
    soapMessage->addHeader("http://www.w3.org/2005/08/addressing", "wsa:Action",
            qualifier);


    // Loopback message
    //----------------------------
    this->sendMessage(soapMessage->toString());

    // Wait For reception (1 sec)
    //-----------------------------
    ASSERT_TRUE(handler->join(1000*1000*1000));

    Logging::getLogger("TestRCPHandler.fail_witness")->infoStream() << "Finished";

    //SOAPMessage msg

}

TEST_F(TestRCPHandler, two_actions) {

    string qualifier1 = "tests.rpchandler.two_actions.action1";
    string qualifier2 = "tests.rpchandler.two_actions.action2";


    // Prepare Action 1
    //--------------------
    RPCHandlerIntermediary *  rpcIntermediary1 =  new RPCHandlerIntermediary();
    this->addressing.addChild(rpcIntermediary1);
    SignalingHandler * handler1 = new SignalingHandler();
    rpcIntermediary1->registerHandlerFactory(qualifier1,handler1);

    rpcIntermediary1->setFilter("tests.rpchandler.two_actions.action1");

    // Prepare Action 2
    //-------------------------
    RPCHandlerIntermediary *  rpcIntermediary2 =  new RPCHandlerIntermediary();
    this->addressing.addChild(rpcIntermediary2);
    SignalingHandler * handler2 = new SignalingHandler();
    rpcIntermediary2->registerHandlerFactory(qualifier2,handler2);

    rpcIntermediary2->setFilter("tests.rpchandler.two_actions.action2");


    // Send Action 1 and 2
    //----------------
    SOAPMessageFactory soapFactory;

    //-- Action1
    SOAPMessage * soapMessage1 = soapFactory.build();
    soapMessage1->addXPathNamespace("wsa","http://www.w3.org/2005/08/addressing");
    soapMessage1->addHeader("http://www.w3.org/2005/08/addressing", "wsa:Action", qualifier1);
    this->sendMessage(soapMessage1->toString());

    //-- Action2
    SOAPMessage * soapMessage2 = soapFactory.build();
    soapMessage2->addXPathNamespace("wsa","http://www.w3.org/2005/08/addressing");
    soapMessage2->addHeader("http://www.w3.org/2005/08/addressing", "wsa:Action", qualifier2);
    this->sendMessage(soapMessage2->toString());

    // Wait for joins on Action 1 and 2
    //--------------------
    EXPECT_TRUE(handler1->join(1000*1000*1000));
    EXPECT_TRUE(handler2->join(1000*1000*1000));

    // Action 1 and 2 semaphores must be empty (to be sure thew messages have only matched one of the intermediaries, and not both)
    //-----------------------------
    EXPECT_EQ(0,handler1->getGrantsCount());
    EXPECT_EQ(0,handler2->getGrantsCount());



}

