/*
 * Logging.cpp
 *
 *  Created on: Nov 7, 2011
 *      Author: rleys
 */

// Includes
//----------------

//-- Std
#include <iostream>
#include <string>
using namespace std;

//-- Log4cpp
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Category.hh>

#include "Logging.h"

namespace OSI {
namespace WSB {

Logging Logging::singleton;

Logging::Logging()  {

	//-- Init Log4Cpp subsystem
	this->appender = new log4cpp::OstreamAppender("Console",&(std::cout));
	log4cpp::Layout* layout = new log4cpp::BasicLayout();
	this->appender->setLayout(layout);


	//-- Create base category
	/*this->baseCategory = &(log4cpp::Category::getInstance(string("OSI.wsb")));
	this->baseCategory->setAdditivity(false);
	this->baseCategory->setAppender(this->appender);
	this->baseCategory->setPriority(log4cpp::Priority::INFO);*/
}

Logging::~Logging() {

   /* this->baseCategory->shutdown();
    delete this->baseCategory;
    delete this->appender;*/

}

Logging * Logging::getInstance() {
	return &singleton;
}

log4cpp::Category * Logging::getLogger(const char * name) {

	//string cname = (Logging::getInstance()->baseCategory->getName()+"."+name);

    string cname = "OSI.wsb.";
    cname+= name;
	log4cpp::Category * newCategory = &(log4cpp::Category::getInstance(cname));
	newCategory->setAdditivity(false);
	newCategory->setAppender(Logging::getInstance()->appender);
	//newCategory->setPriority(log4cpp::Priority::INFO);

	return newCategory;

}

} /* namespace WSB */
} /* namespace OSI */
