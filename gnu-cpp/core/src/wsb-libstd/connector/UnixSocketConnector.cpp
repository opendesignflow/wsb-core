/*
 * UnixSocketConnector.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: rleys
 */

// Includes
//---------------

//-- Std
#include <error.h>
#include <cstring>
using namespace std;

//-- Common
#include <wsb-core/common/Logging.h>

//-- network
#include <wsb-core/network/AbstractConnector.h>

//-- Unix socket
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "UnixSocketConnector.h"

namespace OSI {
namespace WSB {

UnixSocketConnector::UnixSocketConnector() : AbstractConnector("unix://wsb") {

    //-- Defaults
    this->socketDescriptor = 0;

}

UnixSocketConnector::~UnixSocketConnector() {

}

void UnixSocketConnector::init() {

	//-- Create The socket
	this->socketDescriptor = socket(PF_LOCAL,SOCK_DGRAM,0);
	if (!this->socketDescriptor) {
		perror("Could not create UNIX socket");
	}

	//-- Parent call
	AbstractConnector::init();

}

void UnixSocketConnector::reload() {

}

void UnixSocketConnector::stop() {

	//-- Stop socket
	close(this->socketDescriptor);
	unlink("wsb.sock");

	//-- Stop thread
	Thread::stop();
}

void UnixSocketConnector::finish() {

	//-- Stop socket
	close(this->socketDescriptor);

	//-- Stop thread
	Thread::finish();

}

void UnixSocketConnector::maintainConnection(NetworkContext * connection) {

 //-- Let it be

}


void UnixSocketConnector::run() {

	Logging::getLogger("network.connector.unixsocket")->infoStream() << "Starting Unix socket connector";

	// Bind the socket and listen
	//----------------------
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, 108, "#wsb");
	if (bind(this->socketDescriptor,(const sockaddr*)&address,sizeof(address))==-1) {
		perror("Could not bind Unix socket ");
	}
	//listen(this->socketDescriptor,100);

	//-- Prepare socket to send
	int sendSocket = socket(PF_UNIX,SOCK_DGRAM,0);
	if (sendSocket == -1) {
		perror("Could not create send UNIX socket");
		return;
	}

	//-- Prepare Buffers
	//---------------



	//-- To receive source name
	socklen_t nameBufferSize = 4096; // 4 K should be enough
	char nameBuffer[nameBufferSize];

	while (!this->stopRequested()) {

		//-- Wait for connection
		/*int cSocket = accept(this->socketDescriptor,NULL,0);
		if (cSocket==-1) {
			perror("Could not accept connections on Socket");
			break;
		}
		Logging::getLogger("network.connector.unixsocket")->infoStream() << "Accepted connection with client";
*/
		//-- To receive data
		struct iovec msgBuffer;
		msgBuffer.iov_len = 4096; // 4 K should be enough
		msgBuffer.iov_base = calloc(msgBuffer.iov_len,sizeof(char));

		//-- Wait for message
		struct sockaddr_un senderAddress;
		struct msghdr receivedMsg;
		receivedMsg.msg_iov = &msgBuffer;
		receivedMsg.msg_iovlen = 1;
		receivedMsg.msg_controllen = 0;
		receivedMsg.msg_name = &senderAddress;
		receivedMsg.msg_namelen = sizeof(struct sockaddr_un);

		if (recvmsg(this->socketDescriptor,&receivedMsg,MSG_EOR)!=-1) {
		//if (recv(cSocket,msgBuffer.iov_base,msgBuffer.iov_len,0)!=-1) {


			//-- Got message
			string msg((char*)msgBuffer.iov_base);
			Logging::getLogger("network.connector.unixsocket")->infoStream() << "Got from " << senderAddress.sun_path;
			Logging::getLogger("network.connector.unixsocket")->infoStream()  << msg;


			//-- Send Ack
			struct msghdr ackMessage;
			ackMessage.msg_iov = &msgBuffer;
			ackMessage.msg_iovlen = 1;
			ackMessage.msg_controllen = 0;
			ackMessage.msg_name = &address;
			ackMessage.msg_namelen = sizeof(struct sockaddr_un);
			/*if (sendmsg(sendSocket,&ackMessage,0)==-1) {
				perror("Could not  send ACk on Socket");
			}*/


		} else {
			perror("Could not start receiving on Socket");
		}

	}

	//-- Free Things
	//free(msgBuffer.iov_base);


	Logging::getLogger("network.connector.unixsocket")->infoStream() << "Stopping Unix socket connector";

}


void UnixSocketConnector::doSend(MessageContext * context) {

	//-- Serialize message
	string str = context->getMessage()->toString();
	const char * messageChars = str.c_str();

	// Create buffer
	struct iovec msgBuffer;
	msgBuffer.iov_len = strlen(messageChars) * sizeof(char);
	msgBuffer.iov_base = calloc(msgBuffer.iov_len,sizeof(char));
	memcpy(msgBuffer.iov_base,messageChars,msgBuffer.iov_len);

	Logging::getLogger("network.connector.unixsocket")->infoStream() << "Buffer will be oflength: " << msgBuffer.iov_len;

	//-- Prepare
	/*struct msghdr {
	    void         *msg_name;       /* optional address
	    socklen_t     msg_namelen;    /* size of address
	    struct iovec *msg_iov;        /* scatter/gather array
	    size_t        msg_iovlen;     /* # elements in msg_iov
	    void         *msg_control;    /* ancillary data, see below
	    size_t        msg_controllen; /* ancillary data buffer len
	    int           msg_flags;      /* flags on received message
	};
    */
	struct msghdr msg;

	// Destination name
	/*char * socketName = "wsb.sock";
	msg.msg_namelen = strlen(socketName) * sizeof(char);
	msg.msg_name = calloc(strlen(socketName),sizeof(char));
	memcpy(msg.msg_name,socketName,strlen(socketName) * sizeof(char));*/
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, 108, "#wsb");
	msg.msg_name = &address;
	msg.msg_namelen = sizeof(struct sockaddr_un);

	// Data buffer
	msg.msg_iov = &msgBuffer;
	msg.msg_iovlen = 1;

	// Anc data (none)
	msg.msg_controllen = 0;

	//-- Send message
	//-------------------------

	int sock = socket(PF_UNIX,SOCK_DGRAM,0);
	if (sock == -1) {
		perror("Could not create Client UNIX socket");
		return;
	}

	//-- Connect
	//struct sockaddr_un address;
	//address.sun_family = AF_UNIX;
	//sprintf(address.sun_path,"wsb.sock");
	/*if (connect(sock,(const sockaddr*)&address,sizeof(address))==-1) {
		perror("Could not connect");
	}*/

	//-- Send
	if (sendmsg(sock,&msg,MSG_EOR)==-1) {
	//if (::send(sock,msgBuffer.iov_base,msgBuffer.iov_len,0)==-1) {
		perror("Could not send message");
	} else {
		Logging::getLogger("network.connector.unixsocket")->infoStream() << "Message send it seems";
	}

	//-- Wait for ACK
	if(recvmsg(sock,&msg,0)==-1) {
		perror("Could not wait for ackq");
	}

}

} /* namespace WSB */
} /* namespace OSI */
