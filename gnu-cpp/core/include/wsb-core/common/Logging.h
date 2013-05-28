/*
 * Logging.h
 *
 *  Created on: Nov 7, 2011
 *      Author: rleys
 */

#ifndef LOGGING_H_
#define LOGGING_H_

// Includes
//----------------

//-- Std
using namespace std;

//-- Log4cpp
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>

namespace OSI {
namespace WSB {

/**
 * Singleton class to ease access to logger creation
 */
class Logging {


private:

	static Logging singleton;

	/// The Appender
	log4cpp::Appender* appender;

	/// Base category for Broker
	log4cpp::Category * baseCategory;


public:
	Logging();
	virtual ~Logging();


	/**
	 * Returns a Category to log, with the baseCategory name+"." prepended
	 * @return The name of the whished logger
	 */
	static log4cpp::Category * getLogger(const char * name);

	/**
	 * Singleton Retrieval method
	 * @return
	 */
	static Logging * getInstance();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* LOGGING_H_ */
