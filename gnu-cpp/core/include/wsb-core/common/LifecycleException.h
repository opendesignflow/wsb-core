/*
 * LifecycleException.h
 *
 *  Created on: Nov 14, 2011
 *      Author: rleys
 */

#ifndef LIFECYCLEEXCEPTION_H_
#define LIFECYCLEEXCEPTION_H_

// Includes
//--------------------

//-- Std
#include <string>
#include <iostream>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>





namespace OSI {
namespace WSB {

/// Enum to help describe the state of a component managed by a lifecycled
enum LIFECYCLE_STATE {

	LIFECYCLE_INITIAL, //< Initial state, or after finished
	LIFECYCLE_STOPPED, //< Stopped is after initial, of after call to stop
	LIFECYCLE_STARTED, //< If can be concidered as running

};


/**
 * Thrown if an error occured relative to the Lifecycle management/synchonisation
 */
class LifecycleException: public Exception {
public:

	enum STATE {

		LIFECYCLE_INITIAL, //< Initial state, or after finished
		LIFECYCLE_STOPPED, //< Stopped is after initial, of after call to stop
		LIFECYCLE_STARTED, //< If can be concidered as running

	};

	LifecycleException();
	LifecycleException(string message);
	virtual ~LifecycleException() throw();



	LifecycleException & operator<<(LIFECYCLE_STATE);
	LifecycleException & operator<<(const char* msg);
	LifecycleException & operator<<(string & );
	LifecycleException & operator<<(int & );


};

} /* namespace WSB */
} /* namespace OSI */



#endif /* LIFECYCLEEXCEPTION_H_ */
