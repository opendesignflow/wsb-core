/**
 * SAML2.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef SAML2_H_
#define SAML2_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

namespace OSI {
namespace WSB {

/**
 * This class contains some static definitions for SAML2 XML usage
 */
class SAML2 {
    public:

        /// Namespace for Assertion elements
        static string NS_ASSERTION;

        /// Namespace for protocol elements
        static string NS_PROTOCOL;

        SAML2();
        virtual ~SAML2();
};

}
}

#endif /* SAML2_H_ */
