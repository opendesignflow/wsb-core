/*
 * Exception.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

// Includes
//----------------

//-- Std
#include <exception>
#include <string>
using namespace std;

namespace OSI {
namespace WSB {

/**
 * A base class for Exceptions in the project
 */
class Exception: public exception {

protected:

	/// The message to render
	string message;

public:
	Exception();
	Exception(string message);
	virtual ~Exception() throw();

	/**
	 * Set message to be returned by this exception
	 * @param message
	 */
	void setMessage(string message);

	/// Streams the char * into the actual message string
	Exception & operator<<(const char*);

	/// Streams into the actual message string
	Exception & operator<<(long unsigned int);

	/// Streams the string into the actual message string
	Exception & operator<<(string &);

	/// Overload of base std::exception to get #message
	virtual const char * what() const throw();

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* EXCEPTION_H_ */
