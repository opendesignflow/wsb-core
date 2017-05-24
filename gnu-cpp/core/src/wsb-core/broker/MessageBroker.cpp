/*
 * MessageBroker.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: rleys
 */
// Includes
//------------------
//-- Engine
#include <wsb-core/engine/WSEngine.h>

//-- Message
#include <wsb-core/message/MessageContext.h>

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>

//-- Logging
#include <wsb-core/common/Logging.h>

#include "MessageBroker.h"

namespace OSI {
namespace WSB {

MessageBroker::MessageBroker(NetworkLayer * networkLayer,
        MessageContext * messageContext, BrokeringTree brokeringTree) :
        Dispatchable() {

    //-- Init
    this->networkLayer = networkLayer;
    this->messageContext = messageContext;
    this->brokeringTree = brokeringTree;

}

MessageBroker::~MessageBroker() {
    // TODO Auto-generated destructor stub
}

void MessageBroker::run() {

    Logging::getLogger("broker.messageBroker")->infoStream()
            << "Brokering Message";

    //-- Up or Down
    if (this->messageContext->getDirection() == MessageContext::UP) {

        try {
            //-- Message goes UP
            //--------------------
            this->brokeringTree.up(this->messageContext);

            //-- After Sending should be required
            if (this->messageContext->getNetworkContext() != NULL) {

                Logging::getLogger("broker.messageBroker")->infoStream()
                        << "Will be sending to "
                        << this->messageContext->getNetworkContext()->getURI()->getURI();

                this->networkLayer->send(messageContext);

            }
        } catch (IntermediaryException& ex) {
            Logging::getLogger("broker.messageBroker")->errorStream()
                    << "Brokering Message failed with error" << ex.what();

            //-- Register error in Message Context
            this->messageContext->setError(ex);

            //-- If we have a network context, send
            if (this->messageContext->getNetworkContext() != NULL) {
                this->networkLayer->send(messageContext);
            }

        }

    } else {

        try {
            //-- Message goes down
            //--------------------------------
            this->brokeringTree.down(this->messageContext);

            //-- Afterwards maybe looping, or error
            //----------------------------

            //-- Error : Send ?
            if (this->messageContext->getError() != NULL
                    && this->messageContext->getNetworkContext() != NULL) {

                //-- Log error
                Logging::getLogger("broker.messageBroker")->errorStream()
                        << "Brokering Message down failed with error: "
                        << this->messageContext->getError()->what();

                //-- Send?
                if (this->messageContext->getNetworkContext() != NULL) {
                    this->networkLayer->send(messageContext);
                }

            }

        } catch (IntermediaryException& ex) {

            //-- Log Error
            Logging::getLogger("broker.messageBroker")->errorStream()
                    << "Brokering Message down fatal failed with error: "
                    << ex.what();

            //-- Register error in Message Context
            this->messageContext->setError(ex);

            //-- If we have a network context, send
            if (this->messageContext->getNetworkContext() != NULL) {
                this->networkLayer->send(messageContext);
            }

        }

    }

}

} /* namespace WSB */
} /* namespace OSI */
