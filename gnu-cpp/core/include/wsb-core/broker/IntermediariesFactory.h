/*
 * IntermediariesFactory.h
 *
 *  Created on: Nov 10, 2011
 *      Author: rleys
 */

#ifndef INTERMEDIARIESFACTORY_H_
#define INTERMEDIARIESFACTORY_H_


// Includes
//---------------------

//-- Std
#include <string>
#include <map>
using namespace std;

//-- Broker
#include <wsb-core/broker/Intermediary.h>
#include <wsb-core/broker/IntermediaryFactory.h>

namespace OSI {
namespace WSB {



/**
 * This class is the access point to create Intermediaries based on their Names.
 * The application should register a Name with a specific factory to allow this Global factory to build a specific intermediary based on his name
 *
 * <h1>Creating an Intermediary</h1>
 * To create an Intermediary of your own, do following
 *
 * 		- Create a class deriving Intermediary interface
 * 		- If you want to have it created dynamicly from XML for example:
 * 			- Create a Factory for your Intermediary, deriving IntermediaryFactory<>
 * 			- Register it along with a Name to this global factory
 *
 * <h1>Usage</h1>
 * Usage is following:
 *
 * - Application part wanting to create an intermediary (From XML for example):
 *
 *   	...
 *   	Intermediary * intermediary = IntermediariesFactory::createIntermediary("MyIntermediary");
 *   	MyIntermediary * myintermediary = IntermediariesFactory::createIntermediary("MyIntermediary");
 *   	...
 *
 * - The Application part responsible for declaring "MyIntermediary" as name to create the class MyIntermediaryImpl (class Name and declared name different to avoid wrong understanding)
 *
 * 		...
 * 		// The Factory is a pointer, and is managed by the factory (deleted when IntermediariesFactory is deleted)
 * 		// Pointer is used to still allow application to configure it for special cases
 *   	IntermediariesFactory::provideIntermediary("MyIntermediary",new MyIntermediaryFactory<>());
 * 		...
 *
 * - MyIntermediaryFactory<>.h
 *
 * 		...
 * 		class MyIntermediaryFactory<> : public IntermediaryFactory<><MyIntermediaryImpl> {
 *
 * 			...
 * 			MyIntermediaryImpl * createIntermediary() {
 * 				return new MyIntermediaryImpl();
 * 			}
 * 			...
 * 		}
 *
 * @warning This class is Thread protected
 *
 */
class IntermediariesFactory {

private:

	/// Singleton reference
	static IntermediariesFactory singleton;

	/// Factories Map
	map<string,IntermediaryFactory * > factoriesMap;

public:
	IntermediariesFactory();

	/**
	 * @warning Destroys all the associated factories
	 */
	virtual ~IntermediariesFactory();

private:

	/**
	 * Get the Singleton
	 * @return
	 */
	static IntermediariesFactory& getInstance();

public:

	/**\defgroup Creation Intermediary Creation methods */
	/**@{*/

	/**
	 * Creates an Intermediary through previously registered factory.
	 * @param name
	 * @return A pointer to the created Intermediary, or NULL if no factory available. User should manage the returned object
	 */
	static Intermediary*  createIntermediary(string name) {

		//-- Prepare result
	    Intermediary* result = NULL;

		//-- Get Instance
		IntermediariesFactory& factories = IntermediariesFactory::getInstance();

		//-- Create if name has a factory
		//IntermediaryFactory<T> * factory = (IntermediaryFactory<T>*) factories.factoriesMap[name];
		IntermediaryFactory * factory = factories.factoriesMap[name];
		if (factory!=NULL) {

			result = factory->createIntermediary();

		}

		//-- LOCK

		//-- UNLOCK

		return result;

	}

	/**}@*/

	/**\defgroup Register Factories Registering methods */
	/**@{*/

	/**
	 * Register An Intermediary Factory under the provided Name
	 * @param name
	 * @param factory
	 * @return true if succeeded, false if the name is already in use
	 */
	static bool provideIntermediary(string name,IntermediaryFactory * factory) {

		//-- Prepare result
		bool result = false;

		//-- Get Instance
		IntermediariesFactory& factories = IntermediariesFactory::getInstance();

		//-- LOCK

		//-- Check name does not exist
		if (factories.factoriesMap[name]==NULL) {

			//-- Record to Map
			factories.factoriesMap[name] = factory;
		}

		//-- UNLOCK


		return result;

	}

	/**
     * Register An Intermediary Factory under the name provided by the factory
     * @param name
     * @param factory
     * @return true if succeeded, false if the name is already in use
     */
    static bool provideIntermediary(IntermediaryFactory * factory) {

        //-- Prepare result
        bool result = false;

        //-- Get Instance
        IntermediariesFactory& factories = IntermediariesFactory::getInstance();

        //-- LOCK

        //-- Check name does not exist
        if (factories.factoriesMap[factory->getSupportedIntermediaryName()]==NULL) {

            //-- Record to Map
            factories.factoriesMap[factory->getSupportedIntermediaryName()] = factory;
        }

        //-- UNLOCK


        return result;

    }

	/**
	 * Deregister the factory for the provided name
	 * @warning The returned factory is evidently not destroyed
	 * @param name
	 * @return The pointer to the deregistered factory, or NULL if no factory was registered
	 */
	static IntermediaryFactory * removeIntermediary(string name) {

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

	}


	/**}@*/

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* INTERMEDIARIESFACTORY_H_ */
