package com.idyria.osi.wsb.core.network.connectors.tcp

import scala.collection.JavaConversions._

import com.idyria.osi.wsb.core.message.HTTPMessage

import com.idyria.osi.wsb.core.network.AbstractConnector
import com.idyria.osi.wsb.core.network.protocols.ProtocolHandler

import com.idyria.osi.wsb.core.broker.MessageBroker
import com.idyria.osi.wsb.core.network._

import com.idyria.osi.tea.listeningpoints.ListeningSupport

import scala.io.Source
import javax.net.ServerSocketFactory
import java.net.InetSocketAddress
import java.io.PrintStream
import java.nio.channels._
import java.nio._

/**

    TCP Connector is a base connector for TCP connections.
    It provides a high level data send/receive interface for the user, and manages connections with the Connector interface



*/
abstract class TCPConnector extends AbstractConnector with ListeningSupport {




    /**
     * Connection address
     */
    var address = "localhost"

    /**
        Connection Port
    */
    var port = 8083

    // Server Runtime Fields
    //-----------

    /**
        Server Socket used for server connection
    */
    var serverSocket : ServerSocketChannel = null

    /**
    * Maps a string to te client handler, for backpath matching
    */
    var clientsContextsMap =  Map[String,TCPNetworkContext]()


   //var onAcceptConnection   = {  }

    // Client Runtime Fields
    //-----------------

    var clientSocket : SocketChannel = null

    var clientNetworkContext : TCPNetworkContext = null


    // LifeCycle
    //-------------------

    /**
        Prepare Socket

    */
    override def lInit = {

        logInfo ("Creating Socket")

        // Create Server Socket
        //----------------------------
        if (this.direction == AbstractConnector.Direction.Server) {
             this.serverSocket = ServerSocketChannel.open();
        }



    }

    /**
        Start Server Socket Thread
    */
    override def lStart = {

        this.start
/*
        // Start Server Thread
        //--------------
        if (this.direction == AbstractConnector.Direction.Server) {
            this.start
        }
        // Connect to server as client
        //---------------------
        else {

            logInfo(s"Starting TCP Connector as client on $address:$port")

            // Connect
            this.clientSocket = SocketChannel.open(new InetSocketAddress(this.address,this.port))

            // Record Network Context
            this.clientNetworkContext = new TCPNetworkContext(this.clientSocket)

            logInfo(s"Client Started")

            @->("client.started")

        }*/

    }


    override def lStop = {


        // Stop Server socket
        //------------
        if (this.direction == AbstractConnector.Direction.Server) {
            this.stopThread = true
            this.serverSocket.close
        }

    }


    // Connector Run Method
    //------------------------

    /**
        Start this connector in Listening mode if in SERVER direction,
        or tries to connect to  target address if in CLIENT direction

    */
    override def run = {

        // Common
        //---------------

        //-- Prepare Read Buffer
        //----------------
        var readBuffer = ByteBuffer.allocate(4096) // (Buffer of a page size per default)

        // Server Mode
        //-------------------
        if (this.direction == AbstractConnector.Direction.Server) {

            @->("server.start")

            logInfo(s"Starting TCP Connector on $port")

            // Bind
            //--------------
            this.serverSocket.bind(new InetSocketAddress(address,port))



            // Register Selector for all operations
            // !! Selector only works on non blocking sockets
            //----------------------
            var selector = Selector.open()
            this.serverSocket.configureBlocking(false);
            this.serverSocket.register(selector,SelectionKey.OP_ACCEPT)


            // Loop on Selection and handle actions
            //------------------
            @->("server.started")
            while(!this.stopThread) {

                var selected = selector.select
                println("Updated selected keys updated with: "+selected+" keys")
                if (selected>0) {

                    var keyIterator = selector.selectedKeys.iterator;
                    while (keyIterator.hasNext) {

                        var key = keyIterator.next();
                        key match {


                            // Accept
                            //--------------------
                            case key if (key.isValid && key.isAcceptable) => {

                                //println("Accepting Connection")

                                var clientSocket = serverSocket.accept



                                // Prepare Network Context
                                //----------------------------
                                var networkContext = new TCPNetworkContext(clientSocket)
                                clientsContextsMap += (networkContext.toString -> networkContext)

                                @->("server.accepted")
                                @->("server.accepted",networkContext)

                                // Register Socket Channel to selector
                                // !! Selector only works on non blocking sockets
                                //-----------------
                                clientSocket.configureBlocking(false);
                                var clientSocketKey = clientSocket.register(selector,SelectionKey.OP_READ,SelectionKey.OP_WRITE)

                                //-- Register NetworkContext with key
                                clientSocketKey.attach(networkContext)



                            }

                            // Read: Pass Read datas to underlying implementation
                            //-----------------
                            case key if (key.isValid && key.isReadable) => {


                                @->("server.read")

                                //-- Take Channel
                                var networkContext = key.attachment().asInstanceOf[TCPNetworkContext]
                                var socketChannel = networkContext.socket

                                // Read Until 0, or < 0 closes the channel
                                //----------
                                var continue = true;
                                while (continue)
                                    socketChannel.read(readBuffer) match {

                                        // Continue Reading
                                        //------------
                                        case readbytes if (readbytes > 0) => {

                                            // Pass Datas to underlying protocol
                                            readBuffer.flip
                                            protocolReceiveData(readBuffer,networkContext)


                                            // Clear Buffer for next read
                                            readBuffer.clear();
                                        }

                                        // Nothing to read
                                        //----------------
                                        case readbytes if (readbytes == 0) => {
                                            continue = false
                                        }

                                        // Close Client Connection
                                        //------------
                                        case readbytes if (readbytes < 0) => {
                                            this.clientsContextsMap -= networkContext.toString
                                            socketChannel.close();
                                            continue = false
                                        }
                                    }

                            }

                            // Fall back
                            //----------------
                            case key => {

                            }

                        }
                        keyIterator.remove

                    }
                }


            } // EOF thread loop

            // Clean
            //----------------
            selector.close


            @->("server.end")
            // Accept Connections
            //---------------------------
            /*try {
                var clientSocket = this.serverSocket.accept();
                if (clientSocket==null) {

                  //-- Stop if connection broken
                  this.stopThread = true

                } else {

                  //-- Launch handler thread and loop
                  logInfo("Handling Connection")
                  //var clientHandler = new ClientHandler(clientSocket)
                  //clientHandler.start();

                }
            } catch {
              case e : Throwable => {
                this.stopThread = true
              }
            }*/
        }
        // Client Mode
        //------------------------
        else {


            logInfo(s"Starting TCP Connector as client on $address:$port")

            // Connect
            this.clientSocket = SocketChannel.open(new InetSocketAddress(this.address,this.port))

            // Record Network Context
            this.clientNetworkContext = new TCPNetworkContext(this.clientSocket)

            logInfo(s"Client Started")

            @->("client.started")

            // Data Loop
            //------------------------
            var continue = true;
            while (continue && !this.stopThread)
                this.clientSocket.read(readBuffer) match {

                    // Continue Reading
                    //------------
                    case readbytes if (readbytes > 0) => {

                        // Pass Datas to underlying protocol
                        readBuffer.flip
                        protocolReceiveData(readBuffer,this.clientNetworkContext)


                        // Clear Buffer for next read
                        readBuffer.clear();
                    }

                    // Close Client Connection
                    //------------
                    case readbytes if (readbytes < 0) => {
                        this.clientSocket.close();
                        continue = false
                    }
                }

        }
    }


    // Protocol Implementation
    //----------------
    def protocolReceiveData(buffer : ByteBuffer,context: TCPNetworkContext)

    def protocolSendData(buffer : ByteBuffer,context: TCPNetworkContext)
}


/**
    This class is an implementation of TCPConnector, handing out application protocol management to a Protocolhandler class

*/
abstract class TCPProtocolHandlerConnector( var protocolHandlerFactory : ( TCPNetworkContext => ProtocolHandler) ) extends TCPConnector {



    // Protocol Implementation
    //----------------
    def protocolReceiveData(buffer : ByteBuffer,context: TCPNetworkContext) = {

        // Receive through Protocol handler
        //---------------
        ProtocolHandler(context,protocolHandlerFactory).receive(buffer)

    }

    def protocolSendData(buffer : ByteBuffer,context: TCPNetworkContext) = {


        ProtocolHandler(context,protocolHandlerFactory).send(buffer)


    }

}


// Network Context
//---------------------
 class TCPNetworkContext(var socket : SocketChannel ) extends NetworkContext {



}
