/**
 * MessageStreamTCPConnector.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------
//-- Std
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <wchar.h>
#include <cstring>
#include <cerrno>

//-- Common
#include <wsb-core/common/io/IOUtils.h>
#include <wsb-core/common/Logging.h>

#include "MessageStreamTCPConnector.h"

namespace OSI {
namespace WSB {

MessageStreamTCPConnector::MessageStreamTCPConnector() :
        TCPConnector("mstream://127.0.0.1") {

}

MessageStreamTCPConnector::~MessageStreamTCPConnector() {

}

void MessageStreamTCPConnector::doReceive(int selectedSocket,int port) {

    string loggerString = "network.connector.tcp.";
    loggerString.append(
            this->getDirection() == AbstractConnector::SERVER ?
                    "server" : "client");
    loggerString.append(".mstream.doReceive");

    Logging::getLogger(loggerString.c_str())->infoStream()
            << "Start Receiving on socket " << selectedSocket;

    // Start receiving length double
    //-------------------------
    size_t messageLength = 0;
    long int readSize = IOUtils::readBytesFromFile(selectedSocket,
            sizeof(size_t), &messageLength);

    if (readSize == sizeof(size_t)) {

        //-- Received Length
        //--------------------
        Logging::getLogger(loggerString.c_str())->infoStream()
                << "Receiving Message of length: " << messageLength;

        //-- Receive Message
        char * messageString = (char *) calloc(messageLength, sizeof(char));
        readSize = IOUtils::readBytesFromFile(selectedSocket,
                messageLength * sizeof(char), messageString);
        //if (readSize == messageLength) {

            //---- Message received -> Convert to UTF8 base string
            //wchar_t * messageString = (wchar_t *) calloc(readSize, sizeof(wchar_t));
            //mbsrtowcs(messageString,(const char**) &message, readSize,NULL);

            Logging::getLogger(loggerString.c_str())->infoStream()
                    << "Received Message: " << messageString;

            //---- Build Message
            Message * message = this->buildMessage(messageString);

            //---- Prepare Network context
            stringstream clientURI;
            clientURI << this->getURI()->getScheme() << "://"
                    << this->getURI()->getHost();
            clientURI << ":" << port;
            NetworkContext * networkContext = new NetworkContext(
                    clientURI.str());
            //NetworkContext * networkContext = new NetworkContext(this->getURI()->getURI());

            Logging::getLogger(loggerString.c_str())->infoStream()
                    << "URI of peer is: " << clientURI.str();

            //---- Offer
            this->offer(message, networkContext);
       // }

    }
    //-- Reading 0 means END of file
    else if (readSize == 0) {

        // Search and remove peer
        for (map<int, int>::iterator it = this->peerSockets.begin();
                it != this->peerSockets.end(); it++) {
            if (it->second == selectedSocket) {
                this->peerSockets.erase(it);
                break;
            }
        }

    }
    //-- Error on read -> delete socket because it is probably broken
    else if (readSize < 0) {

        Logging::getLogger(loggerString.c_str())->fatalStream()
                << "Error while reading: " << strerror(errno);
        //this->stop();
        // Search and remove peer
        for (map<int, int>::iterator it = this->peerSockets.begin();
                it != this->peerSockets.end(); it++) {
            if (it->second == selectedSocket) {
                this->peerSockets.erase(it);
                break;
            }
        }
        //break;

    }


}

void MessageStreamTCPConnector::doSend(MessageContext * msg) {

    string loggerString = "network.connector.tcp.";
    loggerString.append(
            this->getDirection() == AbstractConnector::SERVER ?
                    "server" : "client");
    loggerString.append(".mstream.doSend");

    // Select Socket Descriptor based on target port
    //-------------------

    int connectionPort =
            msg->getNetworkContext()->getURI()->getPort() == 0 ?
                    CONNECTOR_TCP_DEFAULT_PORT :
                    msg->getNetworkContext()->getURI()->getPort();

    int selectedSocket = -1;
    if (this->peerSockets.count(connectionPort) > 0) {
        selectedSocket = this->peerSockets[connectionPort];
    }

    if (selectedSocket == -1) {
        Logging::getLogger(loggerString.c_str())->fatalStream()
                << "No Socket registered for target URI:"
                << msg->getNetworkContext()->getURI()->getURI();
        return;
    }

    // Produce Message String
    //--------------------------
    string serialized = msg->getMessage()->toString();
    const char* messageString = serialized.c_str();

//    Logging::getLogger(loggerString.c_str())->infoStream()
//                                             << "Sending: " << messageString ;

    // Send Number of bytes
    //-----------------
    size_t length = strlen(messageString);

    Logging::getLogger(loggerString.c_str())->infoStream()
            << "Sending message of length: " << length << " bytes, to "
            << selectedSocket;

    int res = IOUtils::writeBytesToFile(selectedSocket, sizeof(size_t),
            &length);
    if (res < 0) {

        Logging::getLogger(loggerString.c_str())->fatalStream()
                << "Error while writing message length: " << strerror(errno);

    } else {

        // Send Message
        //-----------------
        res = IOUtils::writeBytesToFile(selectedSocket, length,
                messageString);
        if (res < 0) {
            Logging::getLogger(loggerString.c_str())->fatalStream()
                    << "Error while writing message: " << strerror(errno);

        }

        Logging::getLogger(loggerString.c_str())->infoStream()
                << "Finished sending message of length: " << length << " bytes";

    }

}

void MessageStreamTCPConnector::reload() {

}

} /* namespace WSB */
} /* namespace OSI */
