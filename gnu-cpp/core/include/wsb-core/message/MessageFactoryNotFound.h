/*
 * MessageFactoryNotFound.h
 *
 *  Created on: Nov 8, 2011
 *      Author: rleys
 */

#ifndef MESSAGEFACTORYNOTFOUND_H_
#define MESSAGEFACTORYNOTFOUND_H_

// Includes
//---------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

namespace OSI {
namespace WSB {

class MessageFactoryNotFound: public Exception {
    public:
        MessageFactoryNotFound();
        MessageFactoryNotFound(string message);
        MessageFactoryNotFound(const MessageFactoryNotFound& ex) : Exception(ex.message) {

        }
        virtual ~MessageFactoryNotFound() throw();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGEFACTORYNOTFOUND_H_ */
