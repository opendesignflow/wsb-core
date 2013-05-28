/**
 * DOMImplementation.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef DOMIMPLEMENTATION_H_
#define DOMIMPLEMENTATION_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- VDOM
#include <vxml/vdom/VDOMImplementationFactory.h>

//-- LibXML2
#include <libxml2/libxml/parser.h>

namespace VDOM {
namespace LIBXML2 {

/**
 * DOMImplementation entry point for Lib XML2
 */
class DOMImplementation: public VDOM::VDOMImplementation {
    public:
        DOMImplementation();
        virtual ~DOMImplementation();





        /** \defgroup Implementation Implementation Methods */
        /** @{ */

        /**
         * @see VDOM::VDOMImplementation
         * @warning The user is owner of the pointer
         * @param in
         * @return
         */
        virtual VDOMDocument * newDocument();

        /**
         * @see VDOM::VDOMImplementation
         * @warning The user is owner of the pointer
         * @param in
         * @return
         */
        virtual VDOMDocument * parseDocument(string& xml);


        /**
         * @see VDOM::VDOMImplementation
         * @warning The user is owner of the pointer
         * @return
         */
        virtual VDOMLSSerializer * getSerializer();

        /** @} */

};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* DOMIMPLEMENTATION_H_ */
