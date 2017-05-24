/**
 * VDOMRegistry.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

// Includes
//-----------------

//-- Std
 #include <sstream>
 #include <stdexcept>

#include "VDOMRegistry.h"

namespace VDOM {

VDOMRegistry * VDOMRegistry::singleton = NULL;

VDOMRegistry::VDOMRegistry() {
    // TODO Auto-generated constructor stub

}

VDOMRegistry::~VDOMRegistry() {

    //-- Clear implementations
    for (map<string,VDOMImplementationFactory*>::iterator it = this->implementationFactories.begin(); it!=this->implementationFactories.end();it++) {
        delete (*it).second;
    }
    this->implementationFactories.clear();

}



VDOMRegistry & VDOMRegistry::getInstance() {

    if (VDOMRegistry::singleton==NULL) {
        VDOMRegistry::singleton = new VDOMRegistry();
    }

    return *(VDOMRegistry::singleton);
}


VDOMImplementationFactory* VDOMRegistry::registerImplementation(VDOMImplementationFactory* factory) {

    //-- Get actual value
    VDOMImplementationFactory* actual = this->implementationFactories[string(factory->getName())];

    //-- Register
    this->implementationFactories[factory->getName()] = factory;


    return actual;
}


VDOMImplementation * VDOMRegistry::getImplementationForName(const char * name) {

    //-- Look for a factory
    VDOMImplementationFactory * factory =  this->implementationFactories[string(name)];

    //-- Create or return NULL if none
    if (factory==NULL) {

        //-- Throw an exeption
        stringstream ss;
        ss << "Requested VDOM Implementation : " << name << " was not registered. Your application should register it, or check the identifier isn't wrong";
        throw invalid_argument(ss.str().c_str());

        //return NULL;
    } else {
        return factory->createInstance();
    }

}


} /* namespace VDOM */
