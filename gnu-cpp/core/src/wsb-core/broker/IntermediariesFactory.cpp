/*
 * IntermediariesFactory.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

// Includes
//----------------------

//-- Std
#include <string>
using namespace std;

//-- Broker
#include <wsb-core/broker/IntermediaryFactory.h>
#include <wsb-core/broker/Intermediary.h>

#include "IntermediariesFactory.h"

namespace OSI {
namespace WSB {

IntermediariesFactory IntermediariesFactory::singleton;

IntermediariesFactory::IntermediariesFactory() {
	// TODO Auto-generated constructor stub

}

IntermediariesFactory::~IntermediariesFactory() {
	//-- Destroy All registered Factories

}

IntermediariesFactory& IntermediariesFactory::getInstance() {

	return IntermediariesFactory::singleton;

}

//template<> Intermediary* IntermediariesFactory::createIntermediary(string name)

/*
template<class T> T * IntermediariesFactory::removeIntermediary(string name) {

	//-- Prepare result
	IntermediaryFactory * result = NULL;

	//-- Get Instance
	IntermediariesFactory& factories = IntermediariesFactory::getInstance();

	//-- LOCK

	//-- Remove if name exists
	if (factories.factoriesMap[name]!=NULL) {

		//-- Record to Map
		result = factories.factoriesMap.at(name);
		factories.factoriesMap.erase(name);
	}

	//-- UNLOCK

	return result;

}*/


} /* namespace WSB */
} /* namespace OSI */
