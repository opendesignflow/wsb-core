/*
 * BrokeringTree.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

// Includes
//----------------
//-- Std
#include <string>
#include <utility>
#include <typeinfo>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>
#include <wsb-core/common/SGC.h>

//-- Dispatch
#include <wsb-core/common/dispatch/ParallelDispatchQueue.h>
#include <wsb-core/common/dispatch/SerialDispatchQueue.h>

//-- Sync
#include <wsb-core/common/dispatch/MutexLocker.h>

//-- XML
#include <wsb-core/common/xml/XMLBuilderFactory.h>
#include <wsb-core/common/xml/DOMManipulator.h>

//-- Broker
#include <wsb-core/broker/IntermediariesFactory.h>
#include <wsb-core/broker/Intermediary.h>
#include <wsb-core/broker/Group.h>

#include "BrokeringTree.h"


namespace OSI {
namespace WSB {



BrokeringTree::BrokeringTree() {

    //-- Prepare Dispatch Queues
    this->dispatchParallelQueue = new ParallelDispatchQueue();
    //this->dispatchParallelQueue->start();
    this->dispatchSerialQueue = new SerialDispatchQueue();
    //this->dispatchSerialQueue->start();

}

BrokeringTree::~BrokeringTree() {
    // TODO Auto-generated destructor stub
}

void BrokeringTree::readInFromString(string xml) {

    //-- Build XML
    //-- FIXME with validation
    LGC<XMLBuilder> builder = XMLBuilderFactory::getInstance()->getBuilder();
    LGC<VDOM::VDOMDocument> treeDocument = builder->parseString(xml);

    if(treeDocument.getReference()==NULL) {
        Logging::getLogger("broker.brokeringTree.readInFromString")->critStream() << "Provided tree string is not valid: " << xml;
        return;
    }

    //-- Go through Intermediaries with list based Recursivity
    //--------------------------------------

    //-- Prepare data

    // Recursivity pairs: Intermediary=(parent or NULL if root) , DOMElement=base
    pair<Intermediary*, VDOM::VDOMElement*> currentRoot(this,
            treeDocument->getDocumentElement());

    // Recursivity list
    list<pair<Intermediary*, VDOM::VDOMElement*>> recursiveList;

    // Startup with base
    recursiveList.push_back(currentRoot);

    //-- Go on With main Loop
    while (recursiveList.size() > 0) {

        //-- Take currentRoot
        currentRoot = recursiveList.front();

        Logging::getLogger("broker.brokeringTree.readInFromString")->debugStream()
                << "Intermediary root: "
                << currentRoot.second->getLocalName();

        //-- Search All child intermediaries
        LGC<VDOM::VDOMNodeList>  intermediaries = currentRoot.second->getChildNodes();

        //DOMNodeList * intermediaries = currentRoot.second->getElementsByTagNameNS(BROKERING_TREE_NS,X("Intermediary"));
        for (int i = 0; i < intermediaries->getLength(); i++) {

            VDOM::VDOMNode * node = intermediaries->getItem(i);

            // Only proceed elements
            if (node->getNodeType() != VDOM::VDOMNode::ELEMENT_NODE)
                continue;

            // Get Node
            VDOM::VDOMElement * intermediaryNode =
                    static_cast<VDOM::VDOMElement*>(node);

            // Only proceed elements in correct namespace
            if (intermediaryNode->getNamespaceURI().compare(BROKERING_TREE_NS) != 0)
                continue;

            // Result
            Intermediary * intermediary = NULL;

            //---- Intermediary
            //----------------------------
            if (intermediaryNode->getLocalName().compare("Intermediary") == 0) {


                //-- Get Intermediary Name
                string name = intermediaryNode->getAttribute("name");

                Logging::getLogger("broker.brokeringTree.readInFromString")->debugStream()
                                << "Child Intermediary: "<< name;

                //-- Create Intermediary using Factory -> Ignore if could not be created
                intermediary =
                        IntermediariesFactory::createIntermediary(name);
                if (intermediary == NULL) {
                    Logging::getLogger("broker.brokeringTree")->critStream() << "Intermediary with name: " << name << " could not be created, ignoring subtree";
                    continue;
                }



            }
            //---- Group
            //--------------------
            else if (intermediaryNode->getLocalName().compare("Group") == 0) {

                //-- Create a Group Intermediary
                intermediary = new Group();

            }

            //-- Common Processing
            if (intermediary!=NULL) {

                //-- Common Attributes
                //-------------------


                //-- Set Id
                if (intermediaryNode->hasAttribute("id")) {
                    intermediary->setId(intermediaryNode->getAttribute("id"));

                    // Index
                    this->intermediariesIndex[intermediary->getId()] = intermediary;
                }

                //-- Set Name
                if (intermediaryNode->hasAttribute("name")) {
                    intermediary->setName(intermediaryNode->getAttribute("name"));
                }


                //-- Set Filter
                if (intermediaryNode->hasAttribute("filter")) {
                    intermediary->setFilter(intermediaryNode->getAttribute("filter"));
                }

                //-- Set into parent
                //---------------
                currentRoot.first->addChild(intermediary);

                //-- Now set the Intermediary as new possible recursivity base
                //--------------
                recursiveList.push_back(
                        pair<Intermediary*, VDOM::VDOMElement*>(intermediary,
                                intermediaryNode));

            }





        } // EOF intermediaries

        //-- Remove recursive pair we have just processed
        recursiveList.pop_front();

    } // EOF main loop

}

void BrokeringTree::down(MessageContext * context) throw (IntermediaryException) {

    //-- Lock Operation while downing
    MutexLocker(&(this->thread_sync));

    //-- Implement list recursivity, but don't dispatch already (to limit threading)
    list<pair<Intermediary*, void*>> intermediaries;

    // Do
    //------------------------

    //-- Startup with this children
    list<Intermediary*>::iterator it;
    for (it = this->children.begin(); it != this->children.end(); it++) {
        intermediaries.push_back(pair<Intermediary*, void*>((*it), (void*)NULL));
    }

    //-- List
    while (intermediaries.size() > 0) {

        //-- Take up Front
        pair<Intermediary*, void*> current = intermediaries.front();
        Intermediary* currentIntermediary = current.first;
        intermediaries.pop_front();

        //-- Filter if necessary
        //--  * If a filter is defined, the message qualifier must match it. If not, continue and ignore the intermediary
        //----------------
        if (currentIntermediary->getFilter() != NULL) {

            string qualifier = context->getMessage()->getQualifier();

            //-- Match against regexp (0 means success)
            if (regexec(currentIntermediary->getFilter(), qualifier.c_str(), 0,
                    0, 0) != 0) {

                // Non success = continue (ignore)
                continue;

            }

        }

        //-- Call down
        //-------------------------
        try {

            currentIntermediary->down(context);

        } catch (IntermediaryException& ex) {

            //-- Register error in Message Context
            context->setError(ex);

            //-- If we have a network context, probably sending error later, so reup
            if (context->getNetworkContext() != NULL) {
                this->up(context);
            }

        } catch (ResponseException& ex) {

            //-- If we have a network context, probably sending response, so reup
            if (context->getNetworkContext() != NULL) {



                //-- Create New MessageContext
                MessageContext * responseContext = new MessageContext(
                        ex.getResponseMessage(), context->getNetworkContext());

                context->setResponseContext(responseContext);




            }

        }

        //-- Add Children to recursive list
        for (it = currentIntermediary->getChildren().begin();
                it != currentIntermediary->getChildren().end(); it++) {
            intermediaries.push_back(pair<Intermediary*, void*>((*it),(void*) NULL));
        }

    }

}

void BrokeringTree::up(MessageContext * context) throw (IntermediaryException) {
    this->up(context, NULL);
}

void BrokeringTree::up(MessageContext * context, Intermediary * stop)
        throw (IntermediaryException) {

    //-- Lock Operation while downing
    MutexLocker(&(this->thread_sync));

    //-- If we are uping a down message, make it first loop
    if (context->getDirection() == MessageContext::DOWN) {
        context->loop();
    }

    //-- Implement list recursivity and reverse order to upstack
    list<pair<Intermediary*, void*>> intermediaries;

    //-- Resulting reversed list
    list<Intermediary*> upstack;

    // Take care of payload
    // - Format error if there is one
    //-------------------------------
    if (context->getError() != NULL) {
        context->getMessage()->formatError(context->getError());
    }

    // Reverse Order
    //------------------------

    //-- Startup with this children
    list<Intermediary*>::iterator it;
    for (it = this->children.begin(); it != this->children.end(); it++) {
        intermediaries.push_back(pair<Intermediary*, void*>((*it),(void*) NULL));
    }

    //-- List
    bool doStop = false;
    while (intermediaries.size() > 0) {

        //-- Take up Front
        pair<Intermediary*, void*> current = intermediaries.front();
        Intermediary* currentIntermediary = current.first;
        intermediaries.pop_front();

        //-- Add to reverse stack
        upstack.push_front(currentIntermediary);

        //-- Verify Given stop is not into children
        //----------------------
        if (stop != NULL) {

            for (it = currentIntermediary->getChildren().begin();
                    it != currentIntermediary->getChildren().end(); it++) {

                if (*it == stop) {
                    doStop = true;
                    break;
                }
            }
        }

        //-- Add Children to recursive list only if not stopping
        //-- So if stopping, the recursive list can empty
        //---------------
        if (!doStop)
            for (it = currentIntermediary->getChildren().begin();
                    it != currentIntermediary->getChildren().end(); it++) {

                //-- FIXME Filtering heuristic for each children

                intermediaries.push_back(
                        pair<Intermediary*, void*>((*it), (void*)NULL));
            }

    }

    // Destack upstack from front, and callup
    //-------------------------------------
    list<Intermediary*>::iterator upstackit;
    for (upstackit = upstack.begin(); upstackit != upstack.end(); upstackit++) {

        //--  Get Intermediary
        Intermediary * currentIntermediary = *upstackit;

        //-- Call up
        //-------------------------
        try {
            currentIntermediary->up(context);
        } catch (IntermediaryException& ex) {

            //-- If already reporting and error, just log
            //-------------------

            //-- Log error
            Logging::getLogger("broker.brokeringTree")->warnStream()
                    << "Exception while upping on an intermediary "
                    << ex.what();

            if (context->getError() == NULL) {

                //-- Register error in Message Context
                context->setError(ex);

                //-- If we have a network context, probably sending error later, so reup
                if (context->getNetworkContext() != NULL) {
                    this->up(context);
                }

            }

        }

    } // EOF destack

}


Intermediary * BrokeringTree::getIntermediaryFromId(string id) {

    //-- Try to find
    if (this->intermediariesIndex.count(id)>0) {
        return this->intermediariesIndex[id];
    }

    return NULL;

}


DOMManipulator  BrokeringTree::toXML() {

    //-- Setup result
    DOMManipulator result;
    VDOM::VDOMElement * root = result.addElement(BROKERING_TREE_NS,"BrokeringTree");

    //-- Implement list recursivity, to follow
    list<pair<Intermediary*, VDOM::VDOMElement *>> intermediaries;

    // Do
    //------------------------

    //-- Startup with this children
    list<Intermediary*>::iterator it;
    for (it = this->children.begin(); it != this->children.end(); it++) {
        intermediaries.push_back(pair<Intermediary*, VDOM::VDOMElement *>((*it), root));
    }

    //-- List
    while (intermediaries.size() > 0) {

        //-- Take up Front
        pair<Intermediary*, VDOM::VDOMElement*> current = intermediaries.front();
        Intermediary* currentIntermediary = current.first;
        intermediaries.pop_front();

        //-- Add To current Parent
        VDOM::VDOMElement * intermediaryElement = NULL;
        if (typeid(*currentIntermediary) == typeid(Group)) {
            intermediaryElement = result.createElement(BROKERING_TREE_NS,"Group",current.second);

        } else
            intermediaryElement = result.createElement(BROKERING_TREE_NS,"Intermediary",current.second);

        //-- Set id , name and filter
        if (currentIntermediary->getId().size()>0) {
            intermediaryElement->setAttribute("id",currentIntermediary->getId());
        }

        if (currentIntermediary->getFilter()!=NULL) {
            intermediaryElement->setAttribute("filter","afilter");
        }

        if (currentIntermediary->getName().size()>0) {
            intermediaryElement->setAttribute("name",currentIntermediary->getName());
        }


        //-- Add Children to recursive list
        for (it = currentIntermediary->getChildren().begin();
                it != currentIntermediary->getChildren().end(); it++) {
            intermediaries.push_back(pair<Intermediary*, VDOM::VDOMElement*>((*it),intermediaryElement));
        }

    }


    //-- Return
    return result;


}

} /* namespace WSB */
} /* namespace OSI */
