/*
 * TCPConnector.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: rleys
 */

// Includes
//----------------------

//-- Std
#include <string>
#include <cstring>
#include <cerrno>
#include <cstdio>
using namespace std;

//-- Common
#include <wsb-core/common/URI.h>
#include <wsb-core/common/Logging.h>

//-- Network
#include <wsb-core/network/AbstractConnector.h>
#include <wsb-core/network/ConnectorException.h>

//-- Unix socket
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>

#include "TCPConnector.h"

namespace OSI {
namespace WSB {

TCPConnector::TCPConnector(string url) throw (ConnectorException)
        : AbstractConnector(url) {

    //-- Defaults
    this->socketDescriptor = 0;
    this->socketType = 0;

    //-- Init peer sockets semaphore
    sem_init(&(this->peerSocketCreated), 0, 0);


    //-- Init Client socket connectors array
    //bzero(&(this->clientSockets),128*sizeof(int));

    //-- Verify the scheme be tcp or tcps
   /* if (this->getURI()->getScheme()!="tcp" && this->getURI()->getScheme()!="tcps") {
        throw ConnectorException("Scheme can only be tcp or tcps");
    }*/

}

TCPConnector::~TCPConnector() {
	// TODO Auto-generated destructor stub
}

void TCPConnector::reload() {

}
void TCPConnector::init() {

    // Create The socket
    //---------------------------

    //-- Make TCP/IP connection
    if (this->getURI()->getHost().length()>0) {
        this->socketDescriptor = socket(PF_INET,SOCK_STREAM,0);
        this->socketType = PF_INET;
    } else if (this->getURI()->getPath().length()>0) {
        this->socketDescriptor = socket(PF_LOCAL,SOCK_STREAM,0);
        this->socketType = PF_LOCAL;
    } else {
        //-- No Host/File path description
        throw ConnectorException("URI "+this->getURI()->getURI()+" does not describe a Host or File path with which to create a socket");
    }

    if (!this->socketDescriptor) {
        perror("Could not create socket");
    }

    // Parent call
    //-----------------------
    AbstractConnector::init();

}

void TCPConnector::stop() {

    this->getLogger("network.connector.tcp")->infoStream() << "Closing Connector";

    //-- Stop socket
    shutdown(this->socketDescriptor,SHUT_RDWR);
    close(this->socketDescriptor);

    //-- Stop thread
    Thread::stop();
}

void TCPConnector::finish() {

    //-- Stop socket
    //close(this->socketDescriptor);

    //-- Stop thread
    Thread::finish();

}

void TCPConnector::run() {

    //--------------------------
    // Server
    //--------------------------
    if (this->getDirection() == AbstractConnector::SERVER) {

        // Prepare server address
        //------------------------------
        struct sockaddr * address = NULL;

        //-- INET
        if (this->socketType==PF_INET) {

            //-- IP address bzw hostname
            struct sockaddr_in iaddress;
            iaddress.sin_family = AF_INET;
            iaddress.sin_addr.s_addr = INADDR_ANY;
            if (this->getURI()->getPort()>0) {
                iaddress.sin_port = htons(this->getURI()->getPort());
            } else
                iaddress.sin_port = htons(CONNECTOR_TCP_DEFAULT_PORT);

            Logging::getLogger("network.connector.tcp.server")->infoStream() << "Starting INET TCP connector on port " << ntohs(iaddress.sin_port) ;

            address = (struct sockaddr*) calloc(1,sizeof(struct sockaddr_in));
            memccpy(address,&iaddress,1,sizeof(struct sockaddr_in));

        }
        //-- Unix
        else if (this->socketType==PF_LOCAL) {

            //-- Unix Address
            struct sockaddr_un uaddress;
            uaddress.sun_family = AF_UNIX;
            snprintf(uaddress.sun_path, 108, "s%",this->getURI()->getPath().c_str());

            Logging::getLogger("network.connector.tcp.server")->infoStream() << "Starting Local TCP connector" ;


        } else {
            //-- Don't start
            return;
        }

        // Bind the socket and listen
        //----------------------
        int reUseAddress = 1;
        setsockopt(this->socketDescriptor,SOL_SOCKET,SO_REUSEADDR,&reUseAddress,sizeof(int));


        //-- Do bind
        if (bind(this->socketDescriptor,(const sockaddr*)address,sizeof(struct sockaddr))==-1) {
            perror("Could not bind socket");
        }
        listen(this->socketDescriptor,100);




        // Select:
        // - accept event on main socket
        // - doReceive to make a receive round on the connected sockets
        //---------------------------
        this->signalStarted();

        // Start Looping
        while (!this->stopRequested()) {

            //-- Create Sockets to be polled array:
            //--   * 0 = main socketDescriptor for connections
            //--   * 1-... = connected client sockets
            int socketsCount = this->peerSockets.size()+1; // Number of clients to dynamically adjust polled sockets
            struct pollfd  fds[socketsCount];
            //struct pollfd * fds = (struct pollfd * ) calloc(socketsCount,sizeof(struct pollfd));

            // Add Local server socket for connection accept
            fds[0].fd = this->socketDescriptor;
            fds[0].events = POLLIN;

            // Add peer sockets
            int peersCount = 0;
            for ( map<int,int>::iterator it = this->peerSockets.begin(); it != this->peerSockets.end(); it++) {
                fds[peersCount+1].fd = it->second;
                fds[peersCount+1].events = POLLIN;
                peersCount++;
            }

            //-- Poll
            int pollres = ::poll(fds, socketsCount, -1);

            //-- res < 0 -> error -> quit
            if (pollres<0) {
                Logging::getLogger("network.connector.tcp.server")->errorStream() << "Polling returned an error " << strerror(errno);
                break;
            }
            //--  res == 0 -> nothing then loop
            else if (pollres==0) {
                continue;
            }
            //-- res > 0 -> loop on array to answer to events on various sockets
            else {

                for (int i = 0 ; i < socketsCount ; i++) {

                    struct pollfd  fd = fds[i];

                    //-- Error or nothing
                    if ( (fd.revents & POLLERR) || (fd.revents &POLLHUP) || (fd.revents & POLLNVAL)) {

                        continue;

                    }
                    //-- POLLIN on main socket descriptor indicates connection
                    else if ( (fd.revents &POLLIN) && fd.fd == this->socketDescriptor) {


                        struct sockaddr_in clientAddress;
                        socklen_t clientAddressLength = sizeof(struct sockaddr_in);
                        int connectionResult = accept(this->socketDescriptor,(sockaddr*)&clientAddress,&clientAddressLength);
                        if (connectionResult>=0) {

                            // Connection Accepted -> Record socket in sockets list
                            //------------------
                            Logging::getLogger("network.connector.tcp.server")->infoStream() << "Client Connection accepted: " << connectionResult ;

                            //-- Get Client port
                            int local_port = 0;
                            struct sockaddr_in client_sin;
                            unsigned int client_addrlen = sizeof(client_sin);
                            if(getsockname(connectionResult, (struct sockaddr *)&client_sin, &client_addrlen) == 0)
                            {

                               local_port = ntohs(clientAddress.sin_port);
                            }

                            //-- Record Socket descriptor in peer map
                            this->peerSockets[local_port] = connectionResult;
                            sem_post(&(this->peerSocketCreated));
                        }

                    }
                    //-- POLLIN on another socket -> receive
                    else if ( fd.revents &POLLIN) {



                        Logging::getLogger("network.connector.tcp.server")->infoStream() << "Data available on socket: " << fd.fd ;


                        // Find back port
                        int port = 0 ;
                        for (map<int, int>::iterator it = this->peerSockets.begin();
                               it != this->peerSockets.end(); it++) {
                           if (it->second == fd.fd) {
                               port = it->first;
                               break;
                           }
                        }

                        this->doReceive(fd.fd,port);

                    }

                }


            }
            // EOF wait on stop


        }

        //int poll(struct pollfd *fds, nfds_t nfds, int timeout);

        // Accept connections
        //-----------------
        /*Logging::getLogger("network.connector.tcp.server")->infoStream() << "Accepting connections ";
        this->signalStarted();

        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(struct sockaddr_in);
        int connectionResult = accept(this->socketDescriptor,(sockaddr*)&clientAddress,&clientAddressLength);
        if (connectionResult>=0) {

            // Connection Accepted -> Start client Thread
            //------------------
            Logging::getLogger("network.connector.tcp.server")->infoStream() << "Client Connection accepted: " << connectionResult ;

            //-- Get Client port
            int local_port = 0;
            struct sockaddr_in client_sin;
            unsigned int client_addrlen = sizeof(client_sin);
            if(getsockname(connectionResult, (struct sockaddr *)&client_sin, &client_addrlen) == 0)
            {

               local_port = ntohs(clientAddress.sin_port);
            }

            //-- Record Socket descriptor
            this->peerSockets[local_port] = connectionResult;
            this->doReceive();



        } else {

            Logging::getLogger("network.connector.tcp.server")->infoStream() << "Closing TCP connector" ;

        }*/

    }
    //--------------------------
    // Client (same as server, without connect accepting polling)
    //--------------------------
    else  {


       Logging::getLogger("network.connector.tcp.client")->infoStream() << "Starting receiving" ;
       //this->doReceive();

        // Select:
        // - doReceive to make a receive round on the connected sockets
        //---------------------------

        // Start Looping
        while (!this->stopRequested()) {

            // If no peers -> wait for one to be offered
            //-----------------
            if (this->peerSockets.size()==0) {
                sem_wait(&(this->peerSocketCreated));
            } else {
                // Try to get something from semaphore to be sure it is empty
                sem_trywait(&(this->peerSocketCreated));
            }

            Logging::getLogger("network.connector.tcp.client")->infoStream() << "There are some peer sockets to receive on: " << this->peerSockets.size() ;

            //-- Create Sockets to be polled array:
            //--   * 0 = main socketDescriptor for connections
            //--   * 1-... = connected client sockets
            int socketsCount = this->peerSockets.size(); // Number of clients to dynamically adjust polled sockets
            struct pollfd  fds[socketsCount];
            //struct pollfd * fds = (struct pollfd * ) calloc(socketsCount,sizeof(struct pollfd));


            // Add peer sockets
            int peersCount = 0;
            for ( map<int,int>::iterator it = this->peerSockets.begin(); it != this->peerSockets.end(); it++) {
                fds[peersCount].fd = it->second;
                fds[peersCount].events = POLLIN;
                peersCount++;
            }

            //-- Poll
            int pollres = ::poll(fds, socketsCount, -1);

            //-- res < 0 -> error -> quit
            if (pollres<0) {
                Logging::getLogger("network.connector.tcp.client")->errorStream() << "Polling returned an error " << strerror(errno);
                break;
            }
            //--  res == 0 -> nothing then loop
            else if (pollres==0) {
                continue;
            }
            //-- res > 0 -> loop on array to answer to events on various sockets
            else {

                for (int i = 0 ; i < socketsCount ; i++) {

                    struct pollfd  fd = fds[i];

                    //-- Error or nothing -> remove peer
                    if ( (fd.revents & POLLERR) || (fd.revents &POLLHUP) || (fd.revents & POLLNVAL)) {

                        Logging::getLogger("network.connector.tcp.client")->errorStream() << "Seen error on peer socket -> erasing it ";

                        for (map<int, int>::iterator it = this->peerSockets.begin();
                                it != this->peerSockets.end(); it++) {
                            if (it->second == fd.fd) {
                                this->peerSockets.erase(it);
                                break;
                            }
                        }
                        continue;

                    }
                    //-- POLLIN on another socket -> receive
                    else if ( fd.revents &POLLIN) {

                        // Find back port
                        int port = 0 ;
                        for (map<int, int>::iterator it = this->peerSockets.begin();
                               it != this->peerSockets.end(); it++) {
                           if (it->second == fd.fd) {
                               port = it->first;
                               break;
                           }
                        }

                        Logging::getLogger("network.connector.tcp.client")->infoStream() << "Data available on socket: " << fd.fd ;
                        this->doReceive(fd.fd,port);
                    }

                }// EOF Loop on sockets after polling


            }// EOF Polling succeeded

        }// EOF main loop

    }
    // EOF Client
}

void TCPConnector::maintainConnection(NetworkContext * networkContext){




    // If no socket descriptor set -> client
    //-------------------------
    if (this->connectorDirection == AbstractConnector::CLIENT && networkContext->getNetworkState() == NetworkContext::DEAD) {


        // Connection port is the one in the URI or the default one
        int connectionPort = networkContext->getURI()->getPort()==0 ? CONNECTOR_TCP_DEFAULT_PORT : networkContext->getURI()->getPort();

        // Do we have a peer socket registered ?
        if (this->peerSockets.count(connectionPort)>0) {

            // Already Connected!
            // Status is ALIVE
            networkContext->setNetworkState(NetworkContext::ALIVE);

            return;
        }

        //-- Not already connected
        Logging::getLogger("network.connector.tcp.client")->infoStream() << " Opening connection to: " << networkContext->getURI()->getURI()<< ",port: " << connectionPort ;


        //-- Prepare Server address
        struct sockaddr_in serverAddress;
        struct hostent * server;
        serverAddress.sin_family = AF_INET;
        server = gethostbyname(networkContext->getURI()->getHost().c_str());

        // Copy address
        memcpy(&( serverAddress.sin_addr.s_addr),server->h_addr_list[0],server->h_length*sizeof(char)) ;

        // Port
        serverAddress.sin_port = htons(connectionPort);


        //-- Open Connection
        //-- Make TCP/IP connection
        int peerSocketDescriptor = socket(PF_INET,SOCK_STREAM,0);
        int connectionResult = connect(peerSocketDescriptor,(struct sockaddr*)&serverAddress,sizeof(struct sockaddr_in));
        if (connectionResult <0 ) {

            Logging::getLogger("network.connector.tcp.client")->fatalStream()
                                               << "Error while Connecting to server: " << strerror(errno) ;

            perror ("Could not connect to server");


        } else {

            //-- Register in peer connection
            this->peerSockets[connectionPort] = peerSocketDescriptor;
            sem_post(&(this->peerSocketCreated));

            //-- Status is ALIVE
            networkContext->setNetworkState(NetworkContext::ALIVE);

            //-- Start thread for receiving
            //this->start();
        }
    }


}


} /* namespace WSB */
} /* namespace OSI */
