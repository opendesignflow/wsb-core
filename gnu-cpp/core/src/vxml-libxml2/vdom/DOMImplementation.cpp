/**
 * DOMImplementation.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Lib XML2
#include <libxml2/libxml/parser.h>


//-- LibXML2 VDOM
#include "DOMDocument.h"
#include "DOMLSSerializer.h"
#include "DOMImplementation.h"

namespace VDOM {
namespace LIBXML2 {

DOMImplementation::DOMImplementation() {
    // TODO Auto-generated constructor stub

}

DOMImplementation::~DOMImplementation() {
    // TODO Auto-generated destructor stub
}

VDOMDocument * DOMImplementation::newDocument() {


    xmlDocPtr doc = xmlNewDoc((const unsigned char*)"1.1");

    return new DOMDocument(doc);

}

VDOMDocument * DOMImplementation::parseDocument(string & xml) {



    xmlDocPtr doc = xmlParseMemory(xml.c_str(),xml.length());
    if (doc==NULL) {
        return NULL;
    }

    return new DOMDocument(doc);

}


VDOMLSSerializer * DOMImplementation::getSerializer() {

    return new DOMLSSerializer();

}

} /* namespace LIBXML2 */
} /* namespace VDOM */
