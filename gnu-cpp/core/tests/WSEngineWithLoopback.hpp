/**
 * ServerWithLoopback.hpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 *
 *
 *
 *
 */

#ifndef SERVERWITHLOOPBACK_HPP_
#define SERVERWITHLOOPBACK_HPP_

// Includes
//-----------------

//-- Wsb
#include <wsb-libstd/connector/LoopbackConnector.h>


//-- Wsb test
#include "BaseTestEngine.hpp"


using namespace OSI::WSB;

/**
 * This class is a base test that starts a WSEngine with a loop back connector
 *
 */
class WSEngineWithLoopback: public BaseTestEngine {

    protected:
        LoopbackConnector * connector;

    public:

        WSEngineWithLoopback() {
            connector = NULL;
        }

        virtual ~WSEngineWithLoopback() {

            //-- Clean
            delete this->connector;

        }

        /**
         * Send a message on the loopback interface
         * @param message
         */
        void sendMessage(string message) {

            this->connector->loopMessage(message);

        }

    protected:

        virtual void SetUp() {



            //-- Register Loopback Connector with SOAP Support
            connector = new LoopbackConnector();
            this->network.addConnector(connector);
            connector->setMessageType("soap");

            //-- Start
            this->init();
            this->start();

        }

        virtual void TearDown() {

            //-- Stop
            this->stop();
            this->finish();
        }

};

#endif /* SERVERWITHLOOPBACK_HPP_ */
