/**
 * Statement.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_

// Includes
//-----------------

//-- XML
#include <wsb-libstd/xml/XMLNode.h>

namespace OSI {
namespace WSB {

class Statement : public XMLNode {
    public:
        Statement();
        Statement(VDOM::VDOMElement * baseElement);
        virtual ~Statement();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* STATEMENT_H_ */
