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
#include <iostream>
using namespace std;

//-- Broker
#include <wsb-core/broker/BrokeringTree.h>
#include <wsb-core/broker/IntermediariesFactory.h>


//-- SAML2
#include <wsb-libstd/xml/saml2/SAML2Assertion.h>

//-- Gtest
#include <gtest/gtest.h>

using namespace OSI::WSB;





/**
 * Read Brokering tree, and checks index
 */
TEST(saml2,generate_assertion) {


    // Create Assertion
    //------------
    SAML2Assertion assertion;

    assertion.initIssueInstant();
    assertion.initId();

    // Issuer
    //--------------
    assertion.setEntityIssuer("test");


    cout << "Assertion: " << endl << assertion.toXMLString() << endl;



}

