/**
 * VXPathResult.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VXPATHRESULT_H_
#define VXPATHRESULT_H_

// Includes
//-----------------

//-- VDOM
#include "../vdom/VDOMNodeList.h"

namespace VDOM {

/**
 * This class stores the results of executing a XPATH request.
 * It is not designed as an interface, and provides the required I/O for the underlying implementations to just fill it up.
 *
 * An implementor could derive this class to have access to the protected fields, set them up, and transparently return the base VXPathResult
 *
 */
class VXPathResult {

    protected:

        /**
         * This is simply a node list to store the Node Set result
         * This result is owner of the nodesList
         */
        VDOMNodeList nodesList;

    public:
        VXPathResult();
        virtual ~VXPathResult();

        /**
         * Just returns the node list
         * @return
         */
        VDOMNodeList & getNodeList();




};

} /* namespace VDOM */
#endif /* VXPATHRESULT_H_ */
