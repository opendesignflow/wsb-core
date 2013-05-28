/**
 * ConnectorException.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//---------------------


// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Common
#include <wsb-core/common/Exception.h>

#include "ConnectorException.h"

namespace OSI {
namespace WSB {

ConnectorException::ConnectorException() {
    // TODO Auto-generated constructor stub

}

ConnectorException::ConnectorException(string msg) : Exception(msg) {
    // TODO Auto-generated constructor stub

}

ConnectorException::~ConnectorException() throw(){
    // TODO Auto-generated destructor stub
}

} /* namespace WSB */
} /* namespace OSI */
