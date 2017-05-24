/**
 * SAML2Assertion.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <ctime>
#include <cstring>
using namespace std;

//-- Uuid
#include <uuid/uuid.h>
//#include <libxml2/libxml/tree.h>
//#include <xmlsec1/xmlsec/base64.h>

//-- SAML2
#include <wsb-libstd/xml/saml2/SAML2.h>



#include "SAML2Assertion.h"


namespace OSI {
namespace WSB {

SAML2Assertion::SAML2Assertion() : XMLNode() {
    this->initBaseElement();

}

SAML2Assertion::SAML2Assertion(VDOM::VDOMElement* baseElement) : XMLNode(baseElement) {
    this->initBaseElement();

}

SAML2Assertion::~SAML2Assertion() {
    // TODO Auto-generated destructor stub
}

void SAML2Assertion::initBaseElement() {

    //-- Register SAML2 namespace(s)
    this->manipulator->getNSResolver()["saml2"] = SAML2::NS_ASSERTION;

    //-- Create base Element if not present
    if (this->baseElement==NULL) {

        this->baseElement = this->manipulator->addElement(SAML2::NS_ASSERTION,"saml2:Assertion");

    }
    //-- Otherwise verify validity
    else {

    }
}


/**
 * Set the @timestamp value to now
 */
void SAML2Assertion::initIssueInstant() {

    //-- Get Current Time
    time_t currentTime = time(NULL);


    //-- Format
    char result[100];
    strftime(result,sizeof(result),"%Y-%m-%dT%X+%Z",localtime(&currentTime));

    //-- Set Using XQuery
    this->baseElement->setAttribute("IssueInstant",result);

}

/**
 * Set the validity duration of the assertion
 * @param seconds
 */
void SAML2Assertion::setValidFor(unsigned int seconds) {

}

/**
 * Generates an ID to be set onto this assetion
 * @return
 */
void SAML2Assertion::initId() {


    //-- Generate
    uuid_t uuid;
    uuid_generate(uuid);

    //-- Set
    //xmlChar*    b64Result = xmlSecBase64Encode(uuid,16,0);

    //this->baseElement->setAttribute(X("id"),X((const char*)b64Result));

}


void SAML2Assertion::setEntityIssuer(string issuer) {

    this->manipulator->addUniqueElement(SAML2::NS_ASSERTION,"saml2:Issuer",issuer);

}


}
}
