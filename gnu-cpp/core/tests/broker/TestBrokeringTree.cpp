/**
 * TestBrokeringTree.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <fstream>
using namespace std;

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>
#include <wsb-core/broker/IntermediariesFactory.h>

//-- Gtest
#include <gtest/gtest.h>

using namespace OSI::WSB;


/**
 * this class is a default dummy intermediary for test purpose
 */
class DummyIntermediary : public Intermediary , public IntermediaryFactory {

    public:

        DummyIntermediary() {

        }

        ~DummyIntermediary() {

        }

        string getSupportedIntermediaryName() {
            return "dummy";
        }

        Intermediary * createIntermediary() {
            return new DummyIntermediary();
        }

        void up(MessageContext * context) throw (IntermediaryException) {

        }

        void down(MessageContext * context) throw (IntermediaryException,ResponseException)  {

        }


};



/**
 * Read Brokering tree, and checks index
 */
TEST(brokering_tree,read_tree) {


    //-- Setup
    //------------
    IntermediariesFactory::provideIntermediary(new DummyIntermediary());

    //-- Create BrokeringTree
    //------------
    BrokeringTree * brokeringTree = new BrokeringTree();

    //-- Read test file
    //------------

    // Get File
    std::ifstream t("tests/broker/test_brokering_tree.xml");

    // Get content
    std::string btreeContent((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    //-- Read in
    //------------
    brokeringTree->readInFromString(btreeContent);


    //-- Verify ID indexing
    //-------------------------

    // top1/2
    ASSERT_TRUE(brokeringTree->getIntermediaryFromId("top1")!=NULL);
    ASSERT_TRUE(brokeringTree->getIntermediaryFromId("top2")!=NULL);



}

