/*
 * HeaderInjectTimestampFactory.h
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

#ifndef HEADERINJECTTIMESTAMPFACTORY_H_
#define HEADERINJECTTIMESTAMPFACTORY_H_

// Includes
//-----------------

//-- Broker
#include <wsb-core/broker/IntermediaryFactory.h>

//-- Standard
#include <wsb-libstd/intermediary/HeaderInjectTimestamp.h>

namespace OSI {
namespace WSB {

class HeaderInjectTimestampFactory : public IntermediaryFactory {

public:
	HeaderInjectTimestampFactory();
	virtual ~HeaderInjectTimestampFactory() ;

	virtual Intermediary * createIntermediary();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* HEADERINJECTTIMESTAMPFACTORY_H_ */
