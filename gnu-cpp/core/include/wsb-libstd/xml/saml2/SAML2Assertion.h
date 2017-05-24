/**
 * SAML2Assertion.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef SAML2ASSERTION_H_
#define SAML2ASSERTION_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- XML
#include <wsb-libstd/xml/XMLNode.h>

namespace OSI {
namespace WSB {

class SAML2Assertion : public XMLNode {
    public:

        SAML2Assertion();
        SAML2Assertion(VDOM::VDOMElement*);
        virtual ~SAML2Assertion();


        /** \defgroup XML XML init/creation methods */
        /**  @{ */

        virtual void initBaseElement();

        /** @} */

        /** \defgroup Setup Methods to setup assertion */
        /**  @{ */

        /**
         * Set the IssueInstant value to now
         */
        void initIssueInstant();

        /**
         * Set the validity duration of the assertion to now + provided seconds
         *
         * @param seconds
         */
        void setValidFor(unsigned int seconds);

        /**
         * Generates an ID to be set onto this assertion
         * We use here the uuid API
         *
         * The set attribute is @ID
         * @return
         */
        void initId();

        /** @} */



        /** \defgroup Issuer Issuer set methods */
       /** @{ */

        /**
         * Sets the <Issuer>issuer</Issuer> Child Element
         *
         * @param issuer
         */
        void setEntityIssuer(string issuer);


       /** @} */

        /** \defgroup Validity Methods to check validity */
        /** @{ */


        /** @} */


        /** \defgroup XML XML Serialisation/Read */
        /** @{ */





        /** @} */

};

}
}

#endif /* SAML2ASSERTION_H_ */
