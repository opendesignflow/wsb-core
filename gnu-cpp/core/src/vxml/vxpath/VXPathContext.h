/**
 * VXPathContext.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef VXPATHCONTEXT_H_
#define VXPATHCONTEXT_H_

// Includes
//-----------------

//-- Std
#include <string>
#include <stdexcept>
using namespace std;

//-- VDOM
#include "../vdom/VDOMElement.h"

//-- VXpath
#include "VXPathResult.h"
#include "VXPathNSContext.h"

namespace VDOM {

/**
 * This class is a base class to implement XPath resolution.
 * It is not intended to be used directly, but derived by implementations.
 */
class VXPathContext {
    public:
        VXPathContext();
        virtual ~VXPathContext();

        /** \defgroup Setup Setup Methods */
        /** @{ */


        /**
         * The Implementation must record the provided prefix as binding to the provided namespace for resolution
         * @param prefix
         * @param nsuri
         */
        virtual void addNamespaceBinding(string prefix,string nsuri) = 0;

        /**
         * Imports the namespaces binding in nsContext map to this xpath context.
         * The default implementation calls #addNamespaceBinding for every pair in the map
         * @param nsContext
         */
        virtual void importNSContext(VXPathNSContext * nsContext);

        /** @} */


        /** \defgroup Evaluation XPATH Evaluation Methods */
        /** @{ */


        /**
         * Evaluates an expression and returns an XPath Result
         * @warning The returned Result is owned by the user
         * @param xpath
         * @return
         * @throw invalid_argument if the provided xpath expression syntax's is not conform
         */
        virtual VXPathResult * evaluate(string xpath) throw (invalid_argument) = 0 ;



        /**
         * Evaluates an expression and returns a string
         * The string is created depending on expression result.
         * For example, if the first result is an attribute, then the attribute value is returned. If it is an Element, its text content
         * @param xpath
         * @return The result as a string. An empty string if no result was found
         */
        virtual string evaluateAsString(string xpath) throw (invalid_argument) = 0 ;


        /**
         * Evaluates an expression and returns an Element, or NULL if none found
         * This method's default implementation calls #evaluate(string) and takes the first result in the resulting nodeset
         * @param xpath
         * @return
         */
        virtual VDOMElement * evaluateAsElement(string xpath);

        /**
         * Evaluates an expression and returns a true/false result.
         * This method's default implementation calls #evaluate(string), and returns true if the resulting nodeset is not empty
         * @param xpath
         * @return
         */
        virtual bool evaluateAsBoolean(string xpath);


        /** @} */



};

} /* namespace VDOM */
#endif /* VXPATHCONTEXT_H_ */
