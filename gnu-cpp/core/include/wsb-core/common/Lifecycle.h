/*
 * Lifecycle.h
 *
 *  Created on: Oct 31, 2011
 *      Author: rleys
 */

#ifndef LIFECYCLE_H_
#define LIFECYCLE_H_

// Includes
//---------------

//-- Common
#include <wsb-core/common/LifecycleException.h>

namespace OSI {
namespace WSB {

/**
 * Provides a set of Virtual methods to have all components that should support a lifecycle answer to the same methods
 */
class Lifecycle {

public:


protected:

	/// Store the current state
	LIFECYCLE_STATE lifecycleState;


public:

	Lifecycle();
	virtual ~Lifecycle();

	/**
	 * Returns the current lifecycle state of the component
	 * @return The STATE, only valid if component righly set it
	 */
	LIFECYCLE_STATE getLifecycleState();


	/** \defgroup Lifecycle Hooks to be implemented */
	/**@{*/

	virtual void init() ;

	virtual void start();

	virtual void reload();

	virtual void stop();

	virtual void finish();

	/**}@*/

protected:

	/** \defgroup Lifecycle State progression */
	/**@{*/

	void lifecycleProgressToStopped() throw (LifecycleException);
	void lifecycleProgressToStarted() throw (LifecycleException);


	/**}@*/

	/** \defgroup Lifecycle State verification */
	/**@{*/

	/**
	 * Checks we are in the givenState
	 * @param givenState
	 * @param caller A string representing the name of the caller, to have a speaking exception
	 */
	void lifecycleAssert(LIFECYCLE_STATE givenState,string caller) throw (LifecycleException);

	/**
	 * Checks we are no in the givenState
	 * @param givenState
	 */
	void lifecycleAssertNot(LIFECYCLE_STATE givenState) throw (LifecycleException);

	/**}@*/

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* LIFECYCLE_H_ */
