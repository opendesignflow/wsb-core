/**
 * VDOMLSSerializer.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VDOMLSSERIALIZER_H_
#define VDOMLSSERIALIZER_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- VDOM
#include "VDOMNode.h"


namespace VDOM {

/**
 * This class implements methods to serialize an XML document
 */
class VDOMLSSerializer {

    public:
        VDOMLSSerializer();
        virtual ~VDOMLSSerializer();

        /**
         * Serializes the provided Document to a string
         * @param baseNode
         * @return
         */
        virtual string toString(VDOMDocument * baseNode) = 0;

};

} /* namespace VDOM */
#endif /* VDOMLSSERIALIZER_H_ */
