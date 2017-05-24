/**
 * VDOMImplementationFactory.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMIMPLEMENTATIONFACTORY_H_
#define VDOMIMPLEMENTATIONFACTORY_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

#include "VDOMImplementation.h"

namespace VDOM {

/**
 * This is a pure virtual class for implementation to provide a factory for their DOMImplementation
 */
class VDOMImplementationFactory {
    public:
        VDOMImplementationFactory();
        virtual ~VDOMImplementationFactory();

        /**
         * Creates a new instance of a VDOMImplementation.
         * The returned pointer must be released by the user
         *
         * @return
         */
        virtual VDOMImplementation * createInstance() = 0;

        /**
         * Returns the name of this implementation
         * @return
         */
        virtual const char *  getName() = 0;

};


} /* namespace VDOM */
#endif /* VDOMIMPLEMENTATIONFACTORY_H_ */
