/**
 * VDOMNodeList.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMNODELIST_H_
#define VDOMNODELIST_H_

// Includes
//-----------------

//-- Std
#include <vector>
#include <stdexcept>
using namespace std;

//-- VDOM
#include "VDOMNode.h"

namespace VDOM {

/**
 * This is a NodeList default Implementation for usage in VDOM context
 * It is a very simple implementatino based on C++ stl list.
 *
 * It can be visited using the stl iterator, or the getLength()/getItem() interface
 * The getItem() is templated to allow easier direct casting
 *
 */
class VDOMNodeList : public std::vector< VDOM::VDOMNode * > {

    public:
        VDOMNodeList();
        virtual ~VDOMNodeList();

        /**
         * Returns the number of nodes in this list
         * @return basically the size of this list
         */
        size_t getLength();

        /**
         * Returns the item at the provided index
         * @param index
         * @return A pointer to the requested item
         * @throws out_of_range if the provided index is out of this list range
         */
        template< class T = VDOMNode > T * getItem(size_t index) throw (out_of_range);

};

} /* namespace VDOM */
#endif /* VDOMNODELIST_H_ */
