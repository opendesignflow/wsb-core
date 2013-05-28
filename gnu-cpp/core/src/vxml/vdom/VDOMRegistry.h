/**
 * VDOMRegistry.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMREGISTRY_H_
#define VDOMREGISTRY_H_

// Includes
//-----------------

//-- Std
#include <map>
using namespace std;

#include "VDOMImplementationFactory.h"

namespace VDOM {

/**
 * The VDOMRegistry is an entry point for a user to request an IMplementation matching its whishes.
 * Any Implementation should register its factory to this registry to make it accessible to the user
 *
 * This class is to be used as a singleton to ease the use
 *
 */
class VDOMRegistry {

    protected:

        /**
         * This map holds the Implementation Factories registered by runtimes
         */
        map<string ,VDOMImplementationFactory*> implementationFactories;

        /**
         * This is the singleton reference
         */
        static VDOMRegistry * singleton;

    private:
        VDOMRegistry();
        virtual ~VDOMRegistry();

    public:

        /**
         * Returns the Singleton
         * @return
         */
        static VDOMRegistry & getInstance();

        /**
         * Registers a new implementation in the registry
         * The Registery becomes owner of the passed in object
         * @param factory
         * @return The pointer to the previously registered factory if one was registered. If none, just returns NULL
         */
        VDOMImplementationFactory* registerImplementation(VDOMImplementationFactory* factory);

        /**
         * Returns a VDOM Implementation by using its registration name.
         * The user is owner of the pointer
         * @param name
         * @return NULL if nor implementation has been registered
         */
        VDOMImplementation * getImplementationForName(const char * name);

};

} /* namespace VDOM */
#endif /* VDOMREGISTRY_H_ */
