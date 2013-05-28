/**
 * DOMLSSerializer.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------


//-- Std
#include <typeinfo>
#include <stdexcept>
#include <sstream>
#include <cstring>
using namespace std;


//-- VDOM
#include<vxml/vdom/VDOMDocument.h>

//-- LibXML2
#include <libxml/parser.h>

#include "DOMDocument.h"
#include "DOMLSSerializer.h"

namespace VDOM {
namespace LIBXML2 {

DOMLSSerializer::DOMLSSerializer() {
    // TODO Auto-generated constructor stub

}

DOMLSSerializer::~DOMLSSerializer() {
    // TODO Auto-generated destructor stub
}

string DOMLSSerializer::toString(VDOMDocument * document) {

     if (strcmp(typeid(*document).name(),"VDOM::LIBXML2::DOMDocument")==0) {

         xmlChar *xmlbuff;
         int buffersize;
         xmlDocDumpFormatMemory(dynamic_cast<VDOM::LIBXML2::DOMDocument*>(document)->getDocumentPointer(), &xmlbuff, &buffersize, 1);

         return string((const char*)xmlbuff,buffersize);

     } else {
         stringstream ss;
         ss << "VDOM::LIBXML2::DOMLSSerialiser can only serialize libxxml2 implementation documents";
         throw runtime_error(ss.str());
     }



}

} /* namespace LIBXML2 */
} /* namespace VDOM */
