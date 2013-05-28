/*
 * TCLInvokeFactory .h
 *
 *  Created on: TODO
 *  Author: rleys
 */

#ifndef TCLINVOKEFACTORY_H_
#define TCLINVOKEFACTORY_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Broker
#include <broker/IntermediaryFactory.h>

//-- Standard
#include "TCLInvoke.h"


namespace EUVNI {
namespace WSB {
namespace TCL {

class TCLInvokeFactory : public IntermediaryFactory<TCLInvoke> {

protected:

	/// base path to search for scripts. Default: "./" (local folder)
	string basePath;

public:
	TCLInvokeFactory();
	virtual ~TCLInvokeFactory();

	void setBasePath(string);
	string getBasePath();

	virtual TCLInvoke * createIntermediary();

};

} // EOF Namespace EUVNI
} // EOF Namespace WSB
} // EOF Namespace TCL



#endif /* TCLINVOKEFACTORY_H_ */

