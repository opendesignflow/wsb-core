/*
 * IntermediaryFactory.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef INTERMEDIARYFACTORY_H_
#define INTERMEDIARYFACTORY_H_

// Class
//----------------

//-- Broker
#include <wsb-core/broker/Intermediary.h>

namespace OSI {
namespace WSB {

/**
 *
 * Interface to create a Factory for a specific Intermediary Type
 *
 */
class IntermediaryFactory {

public:
	IntermediaryFactory() {

	}
	virtual ~IntermediaryFactory() {

	}

	/**
	 * Returns a name identifying the created intermediary
	 * This is useful for fastm fail-free registreation of factory
	 * @return
	 */
	virtual string getSupportedIntermediaryName() = 0;

	/**
	 * The returned Intermediary should  be managed (in memory term) by the user, by can be controlled for configuration or so
	 * @return A pointer to the newly created intermediary
	 */
	virtual Intermediary * createIntermediary() = 0;

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* INTERMEDIARYFACTORY_H_ */
