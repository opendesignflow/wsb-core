/**
 * Subject.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef SUBJECT_H_
#define SUBJECT_H_

// Includes
//-----------------


//-- XML
#include <wsb-libstd/xml/XMLNode.h>

namespace OSI {
namespace WSB {


class Subject  : public XMLNode {
    public:
        Subject();
        Subject(VDOM::VDOMElement * element);
        virtual ~Subject();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* SUBJECT_H_ */
