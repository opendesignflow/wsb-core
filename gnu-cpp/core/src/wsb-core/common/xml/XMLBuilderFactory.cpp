/*
 * XMLBuilderFactory.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

// Include
//--------------------

//-- Common
#include <common/Logging.h>
#include <common/xml/XMLBuilder.h>

//-- Xerces-c
//#include <xqilla/xqilla-dom3.hpp>
//using namespace xercesc;


#include "XMLBuilderFactory.h"

namespace OSI {
namespace WSB {

XMLBuilderFactory * XMLBuilderFactory::singleton = NULL;

XMLBuilderFactory::XMLBuilderFactory() {

	//-- Initialize Xerces-C + XQuilla
	/*try {
		//Logging::getLogger("common.xml.builderFactory")->infoStream() << "Initializing Xqilla";
		XQillaPlatformUtils::initialize();



	  } catch (const XMLException& eXerces) {
		  Logging::getLogger("common.xml.builderFactory")->critStream() << "Error during Xerces-C initialisation.\n"
	              << "Xerces exception message: "
	              << UTF8(eXerces.getMessage());
	  }*/

}

XMLBuilderFactory::~XMLBuilderFactory() {
    //XQillaPlatformUtils::terminate();
}

XMLBuilderFactory * XMLBuilderFactory::getInstance() {
    if (XMLBuilderFactory::singleton == NULL) {
        XMLBuilderFactory::singleton = new XMLBuilderFactory();
        Logging::getLogger("common.xml.builderFactory")->critStream() << "Singleton Creation\n";
    }
	return XMLBuilderFactory::singleton;
}

XMLBuilder * XMLBuilderFactory::getBuilder() {

	//-- Get A Xerces Builder
	XMLBuilder * builder = new XMLBuilder();

	return builder;

}


} /* namespace WSB */
} /* namespace OSI */
