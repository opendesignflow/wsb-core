/*
 * SGC.cpp
 *
 *  Created on: Aug 23, 2011
 *      Author: rleys
 */

// Includes
//------------------

//-- Common
#include <wsb-core/common/Logging.h>

//-- XML
#include <wsb-core/common/xml/XMLBuilder.h>

#include "SGC.h"

namespace OSI {
namespace WSB {


Garbageable::Garbageable() {

}
Garbageable::~Garbageable() {
	//Logging::getLogger("Garbageable")->infoStream() << "Delete Garbageable";
}

LSGC::LSGC() {

}

LSGC::~LSGC() {
	//Logging::getLogger("common.sgc.destruct")->infoStream() << "Delete LSGC";

	list<Garbageable *>::iterator it;
	for (it = this->gRefs.begin(); it != this->gRefs.end();it++) {
		delete(*it);
	}

}

/*template<class T> T * LSGC::toGarbage(T * o) {
	this->gRefs.push_back(o);
	return o;

}*/




} /* namespace WSB */
} /* namespace OSI */
