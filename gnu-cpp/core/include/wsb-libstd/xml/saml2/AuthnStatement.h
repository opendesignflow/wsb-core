/**
 * AuthnStatement.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef AUTHNSTATEMENT_H_
#define AUTHNSTATEMENT_H_

// Includes
//-----------------


//-- XML
#include <wsb-libstd/xml/XMLNode.h>

namespace OSI {
namespace WSB {

class AuthnStatement : public XMLNode {
    public:
        AuthnStatement();
        AuthnStatement(VDOM::VDOMElement * baseElement);
        virtual ~AuthnStatement();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* AUTHNSTATEMENT_H_ */
