/*
 * IntermediaryException.h
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

#ifndef INTERMEDIARYEXCEPTION_H_
#define INTERMEDIARYEXCEPTION_H_

// Includes
//------------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

namespace OSI {
namespace WSB {

class IntermediaryException: public Exception {
    public:
        IntermediaryException();
        IntermediaryException(string message);
        IntermediaryException(const char * message);
        IntermediaryException(IntermediaryException & ex);
        virtual ~IntermediaryException() throw ();

        /// Streams into the actual message string
        IntermediaryException & operator <<(long unsigned int);

        /// Streams the char * into the actual message string
        IntermediaryException & operator <<(const char*);

        /// Streams the string into the actual message string
        IntermediaryException & operator <<(string &);

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* INTERMEDIARYEXCEPTION_H_ */
