/*
 * HeaderInjectTimestampFactory.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

// Includes
//-------------------

//-- Std
#include <string>
using namespace std;

//-- Broker
#include <wsb-core/broker/IntermediariesFactory.h>
#include <wsb-core/broker/IntermediaryFactory.h>

//-- Standard
#include <wsb-libstd/intermediary/HeaderInjectTimestamp.h>

#include "HeaderInjectTimestampFactory.h"

namespace OSI {
namespace WSB {



HeaderInjectTimestampFactory::HeaderInjectTimestampFactory() {
	// TODO Auto-generated constructor stub

}

HeaderInjectTimestampFactory::~HeaderInjectTimestampFactory() {
	// TODO Auto-generated destructor stub
}


Intermediary * HeaderInjectTimestampFactory::createIntermediary() {

	return new HeaderInjectTimestamp();

}

/*
template<> bool IntermediariesFactory::provideIntermediary(string name,HeaderInjectTimestampFactory * f) {
	return IntermediariesFactory::provideIntermediary(name,static_cast<IntermediaryFactory*>(f));
}
*/
//template bool IntermediariesFactory::provideIntermediary<HeaderInjectTimestampFactory>(string name,HeaderInjectTimestampFactory * f);

} /* namespace WSB */
} /* namespace OSI */
