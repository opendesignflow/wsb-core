/**
 * VDOMImplementation.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMIMPLEMENTATION_H_
#define VDOMIMPLEMENTATION_H_

// Includes
//-----------------

//-- Std
#include <istream>
using namespace std;

//-- VDOM
#include "VDOMDocument.h"
#include "VDOMLSSerializer.h"

namespace VDOM {

/**
 * This class is pure virtual, and XML implementations should start over there to provide users an entry point
 */
class VDOMImplementation {

    public:
        VDOMImplementation();
        virtual ~VDOMImplementation();


        /**
         * This method simply creates a brand new enpty document
         * @param in
         * @return
         */
        virtual VDOMDocument * newDocument() = 0;

        /** \defgroup Parsing XML Parsing methods */
        /** @{ */


        /**
         * Calls #parseDocument(string&) with istream converted into a char buffer
         * @param in
         * @return
         */
        virtual VDOMDocument * parseDocument(istream & in);

        /**
         * To be implemented by provider
         *
         * @param xml
         * @return
         */
        virtual VDOMDocument * parseDocument(string& xml) = 0;

        /** @} */


        /** \defgroup Serialize Serialization methods */
        /** @{ */

        /**
         * Returns a Serializer
         * @warning The user is owner of the pointer
         * @return
         */
        virtual VDOMLSSerializer * getSerializer() = 0;


        /** @} */

};

} /* namespace VDOM */
#endif /* VDOMIMPLEMENTATION_H_ */
