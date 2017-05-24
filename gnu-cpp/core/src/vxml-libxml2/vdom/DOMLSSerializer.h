/**
 * DOMLSSerializer.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef DOMLSSERIALIZER_H_
#define DOMLSSERIALIZER_H_

// Includes
//-----------------

#include "VDOMLSSerializer.h"

namespace VDOM {
namespace LIBXML2 {

/**
 * Implementation of serializer for libxml2
 */
class DOMLSSerializer: public VDOM::VDOMLSSerializer {
    public:
        DOMLSSerializer();
        virtual ~DOMLSSerializer();

        /**
         * @see VDOM::VDOMLSSerializer
         * @param baseNode
         * @return
         */
        virtual string toString(VDOMDocument * baseNode);

};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* DOMLSSERIALIZER_H_ */
