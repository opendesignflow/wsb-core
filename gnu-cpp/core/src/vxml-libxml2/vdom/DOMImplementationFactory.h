/**
 * DOMImplementationFactory.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef DOMIMPLEMENTATIONFACTORY_H_
#define DOMIMPLEMENTATIONFACTORY_H_

// Includes
//-----------------

//-- VDOM
#include <vxml/vdom/VDOMImplementationFactory.h>

//-- LibXML2
#include <libxml2/libxml/parser.h>

namespace VDOM {
namespace LIBXML2 {

/**
 * This factory creates instances of the DOMImplementatino implementation.
 * It also inits the underlying library
 */
class DOMImplementationFactory: public VDOM::VDOMImplementationFactory {

    public:
        DOMImplementationFactory();

        virtual ~DOMImplementationFactory();

        /** \defgroup Factory Factory Methods */
        /** @{ */

        /**
         * VDOM::VDOMImplementation
         * @return
         */
        virtual VDOMImplementation * createInstance();

        /**
         * @see VDOM::VDOMImplementation
         * @return
         */
        virtual const char *  getName();

        /** @} */
};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* DOMIMPLEMENTATIONFACTORY_H_ */
