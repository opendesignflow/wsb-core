/**
 * MessageStreamTCPConnector.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef MESSAGESTREAMTCPCONNECTOR_H_
#define MESSAGESTREAMTCPCONNECTOR_H_

// Includes
//-----------------

#include <wsb-libstd/connector/TCPConnector.h>

namespace OSI {
namespace WSB {

/**
 * This connector implements a very simple message stream connector
 * Its goal is to extract the bytes forming a message on the stream.
 *
 * The protocol follows those rules:
 *
 *  1. Wait for 8 bytes forming a double = message length
 *  2. Fetch following bytes up to length into message buffer
 *  3. When reached, pass bytes to message factory and enqueue
 *  4. Loop back to 1
 *
 */
class MessageStreamTCPConnector: public TCPConnector {
    public:


        /**
         * Per default, mstream targets or provides service on following URI:
         * mstream://127.0.0.1
         */
        MessageStreamTCPConnector();
        virtual ~MessageStreamTCPConnector();

    protected:

        /** \defgroup Send interface */
        /** @{ */

        //protected: virtual void maintainConnection(NetworkContext *);
        virtual void doSend(MessageContext *);

        /** @} */

        /** \defgroup Receive Receive interface */
        /** @{ */

        /**
         * To be implemented by protocol implementation
         * In there the implementatin receives and extract messages
         */
        virtual void doReceive(int socketDescriptor,int port);

        /** @} */

        /** \defgroup Lifecycle */
        /** @{ */

    public:
        virtual void reload();

        /** @} */

};

} /* namespace WSB */
} /* namespace OSI */
#endif /* MESSAGESTREAMTCPCONNECTOR_H_ */
