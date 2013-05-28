/*
 * HeaderInjectTimestamp.h
 *
 *  Created on: Nov 11, 2011
 *      Author: rleys
 */

#ifndef HEADERINJECTTIMESTAMP_H_
#define HEADERINJECTTIMESTAMP_H_

// Include
//-----------------


//-- Broker
#include <wsb-core/broker/IntermediaryException.h>
#include <wsb-core/broker/Intermediary.h>


namespace OSI {
namespace WSB {

class HeaderInjectTimestamp: public Intermediary {
public:
	HeaderInjectTimestamp();
	virtual ~HeaderInjectTimestamp();


	virtual void down(MessageContext * context) throw (IntermediaryException);


	virtual void up(MessageContext * context) throw (IntermediaryException);

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* HEADERINJECTTIMESTAMP_H_ */
