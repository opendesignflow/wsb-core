/**
 * Group.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef GROUP_H_
#define GROUP_H_

// Includes
//-----------------

//-- Broker
#include <wsb-core/broker/Intermediary.h>

namespace OSI {
namespace WSB {

class Group: public Intermediary {

    public:
        Group();
        virtual ~Group();

        /** \defgroup Intermediary Up/Down Methods */
        /** @{ */

        /**
         *
         * Do Nothing
         *
         * @param context
         */
        virtual void down(MessageContext * context) throw (IntermediaryException,ResponseException);


        /**
         * Do Nothing
         * @param context
         */
        virtual void up(MessageContext * context) throw (IntermediaryException);


        /** @} */


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* GROUP_H_ */
