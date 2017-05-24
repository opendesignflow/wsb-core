/*
 * TCLInvoke .h
 *
 *  Created on: TODO
 *  Author: rleys
 */

#ifndef TCLINVOKE_H_
#define TCLINVOKE_H_

// Include
//-----------------

//-- Broker
#include <broker/Intermediary.h>


#define NS_TCL "euvni:wsb:tcl"

namespace EUVNI {
namespace WSB {
namespace TCL {

/**
 * INvoke a TCl script, based on Header:
 *
 *
 * <wsbtcl:InvokeTCL xmlns:wsbtcl="euvni:wsb:tcl" script="scriptname.tcl" />
 *
 */
class TCLInvoke : public EUVNI::WSB::Intermediary {

protected:

	/// base path to search for scripts. Default: "./" (local folder)
	string basePath;

public:
	TCLInvoke(string);
	virtual ~TCLInvoke();

	/// To be Implemented
	virtual void down(MessageContext * context) throw (IntermediaryException);

	/// To be Implemented
	virtual void up(MessageContext * context) throw (IntermediaryException);


};

 } // EOF Namespace EUVNI
} // EOF Namespace WSB
} // EOF Namespace TCL

#endif /* TCLINVOKE_H_ */

