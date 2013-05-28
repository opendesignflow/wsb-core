/**
 * VXPathNSContext.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VXPATHNSCONTEXT_H_
#define VXPATHNSCONTEXT_H_

// Includes
//-----------------

//-- Std
#include <map>
#include <string>

namespace VDOM {

/**
 * This NSContext is simply a map to record a set of prefix<->namespaceURI binding
 * It is thought to be used as a binding store, and be imported into Xpath contexes when needed
 *
 */
class VXPathNSContext : public std::map<std::string,std::string> {

    public:
        VXPathNSContext();
        virtual ~VXPathNSContext();
};

} /* namespace VDOM */
#endif /* VXPATHNSCONTEXT_H_ */
