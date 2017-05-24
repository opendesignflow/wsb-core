/*
 * XMLBuilderFactory.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef XMLBUILDERFACTORY_H_
#define XMLBUILDERFACTORY_H_

// Includes
//--------------

//-- XML
#include <wsb-core/common/xml/XMLBuilder.h>

namespace OSI {
namespace WSB {

/**
 * This factory creates XMLBuilders matching a provided implementation.
 * At the moment, only creates Xquilla implementation
 */
class XMLBuilderFactory {

private:

	static XMLBuilderFactory * singleton;

public:
	XMLBuilderFactory();
	virtual ~XMLBuilderFactory();


	/**
	 * Get the singleton instance
	 * @return
	 */
	static XMLBuilderFactory * getInstance();

	/**
	 * Get a new XML Builder with default available implementation
	 *
	 * @return An XMLBuilder, to be managed by user
	 */
	XMLBuilder * getBuilder();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* XMLBUILDERFACTORY_H_ */
