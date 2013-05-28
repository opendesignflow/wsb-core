/**
 * XPathContext.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef XPATHCONTEXT_H_
#define XPATHCONTEXT_H_

// Includes
//-----------------

//-- VXPath
#include <vxml/vxpath/VXPathContext.h>

//-- DOM Impl
#include "../vdom/DOMDocument.h"
#include "../vdom/DOMElement.h"

//-- LibXML2
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

namespace VDOM {
namespace LIBXML2 {

class XPathContext: public VDOM::VXPathContext {


    protected:

        /**
         * Pointer to the xpathContext object
         */
        xmlXPathContextPtr xpathCtx;

        /**
         * Pointer to the document in which we are working
         */
        DOMDocument * document;

        /**
         * Local XPathResult nested class to have access to VXPathResult protected variables
         */
        class XPathResult : public VDOM::VXPathResult {

            public:
                XPathResult();

                /**
                 * The destructor cleans all the saved nodes
                 */
                virtual ~XPathResult();

        };

    public:

        /**
         * Initialises the context using the base document
         *
         * @param document
         */
        XPathContext(VDOM::LIBXML2::DOMDocument * document);

        /**
         * Initialises the context using an element
         * @param document
         */
        XPathContext(VDOM::LIBXML2::DOMElement * element);

        virtual ~XPathContext();


        /**
         * Binds the namespace in the libxml2 xpath context map
         * @param prefix
         * @param nsuri
         */
        virtual void addNamespaceBinding(string prefix,string nsuri);

        /** \defgroup Evaluation XPATH Evaluation Methods */
        /** @{ */

        /**
         * @see VDOM::VXPathContext#evaluate(string)
         */
        virtual VXPathResult * evaluate(string xpath) throw (invalid_argument) ;

        /**
         * @see VDOM::VXPathContext#evaluateAsString(string)
         */
        virtual string evaluateAsString(string xpath) throw (invalid_argument) ;

        /** @} */

};

} /* namespace LIBXML2 */
} /* namespace VDOM */
#endif /* XPATHCONTEXT_H_ */
