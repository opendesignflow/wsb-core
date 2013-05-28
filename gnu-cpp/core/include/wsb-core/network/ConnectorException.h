/**
 * ConnectorException.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef CONNECTOREXCEPTION_H_
#define CONNECTOREXCEPTION_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

//--

namespace OSI {
namespace WSB {

/**
 * Exception thrown by connectors when something wrong happens
 */
class ConnectorException: public Exception {
    public:
        ConnectorException() ;
        ConnectorException(string);
        virtual ~ConnectorException() throw();
};

} /* namespace WSB */
} /* namespace OSI */
#endif /* CONNECTOREXCEPTION_H_ */
