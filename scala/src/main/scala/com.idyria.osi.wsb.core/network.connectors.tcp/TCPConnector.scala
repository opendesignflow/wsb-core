package com.idyria.osi.wsb.core.network.connectors.tcp

import scala.collection.JavaConversions._
import com.idyria.osi.wsb.core.message._
import com.idyria.osi.wsb.core.network.protocols.ProtocolHandler
import com.idyria.osi.wsb.core.broker.MessageBroker
import com.idyria.osi.wsb.core.network._
import com.idyria.osi.tea.listeners.ListeningSupport
import scala.io.Source
import javax.net.ServerSocketFactory
import java.net.InetSocketAddress
import java.io.PrintStream
import java.nio.channels._
import java.nio._
import java.net.Inet4Address
import java.net.InetAddress
import com.idyria.osi.wsb.core.network.connectors.AbstractConnector
import java.net.SocketOption

/**
 *
 * TCP Connector is a base connector for TCP connections.
 * It provides a high level data send/receive interface for the user, and manages connections with the Connector interface
 *
 *
 *
 */
abstract class TCPConnector() extends AbstractConnector[TCPNetworkContext] with ListeningSupport {

  /**
   * Connection address
   */
  var address = "0.0.0.0"

  /**
   * Connection Port
   */
  var port = 8083

  // Server Runtime Fields
  //-----------

  /**
   * Server Socket used for server connection
   *
   * @group server
   */
  var serverSocket: ServerSocketChannel = null

  /**
   * Server Socket Selector
   *
   * @group server
   */
  var serverSocketSelector: Selector = null

  /**
   *
   * Maps a string to te client handler, for backpath matching
   *
   * @group server
   */
  var clientsContextsMap = Map[String, TCPNetworkContext]()

  //var onAcceptConnection   = {  }

  // Client Runtime Fields
  //-----------------

  /**
   * Client Socket to selected Server
   */
  var clientSocket: SocketChannel = null

  /**
   * Client Network Context to selected server
   */
  var clientNetworkContext: TCPNetworkContext = null

  // Send Interface
  //------------------------

  /**
   * Send for server side to client side
   */
  def send(buffer: ByteBuffer, context: TCPNetworkContext) = {

    this.direction match {

      case AbstractConnector.Direction.Client =>

        require(this.clientNetworkContext != null)

        //-- Pass to protocol implementation
        var resBuffer = protocolSendData(buffer, this.clientNetworkContext)

        //-- Send
        this.clientNetworkContext.socket.write(resBuffer)

      case AbstractConnector.Direction.Server =>

        //-- Pass to protocol implementation
        var resBuffer = protocolSendData(buffer, context)

        //-- Send
        while (resBuffer.remaining != 0)
          context.socket.write(resBuffer)

    }

    // println("Sending byte buffer to context")

  }

  /**
   * FIXME
   *
   * Send a message:
   *
   * - Send back to already existing connection
   * - Send through new opening connection
   */
  def send(msg: Message): Boolean = {

    (this.direction, this.clientNetworkContext) match {

      // Server
      //------------------
      case (AbstractConnector.Direction.Server, _) =>

        // Set Message to network context
        //--------
        msg.networkContext("message" -> msg)

        this.send(msg.toBytes, msg.networkContext.asInstanceOf[TCPNetworkContext])
        true

      // Client
      //--------------

      //-- No Client network context, cannot send
      case (AbstractConnector.Direction.Client, null) =>

        throw new RuntimeException("Client is not connected to Host, maybe something happened at connection")

      //-- Client network connection is there, do it
      case (AbstractConnector.Direction.Client, ctx) =>

        this.send(msg.toBytes, ctx)
        true

    }

  }
  /**
   * This connector can handle the message, if the network context is of the right type, and available
   */
  def canHandle(message: Message): Boolean = {

    message.networkContext match {

      // No context
      case ctx if (ctx == null) => false

      // Server, context must be in clients map
      case ctx if (this.direction == AbstractConnector.Direction.Server) => clientsContextsMap.contains(ctx.toString)

      // Client, host and port must match this one
      case ctx if (this.direction == AbstractConnector.Direction.Client) =>
        ctx.qualifier match {

          case NetworkContext.NetworkString(protocol, message, connectionString) if (protocol == this.protocolType && message == messageType) => true
          case _ => false

        }

      case _ => false
    }

  }

  // Protocol Implementation
  //----------------

  def protocolReceiveData(buffer: ByteBuffer, context: TCPNetworkContext): Option[Iterable[Any]]

  /**
   * Calls send Data  through protocol implementation
   *
   * @return A Resulting ByteBuffer to be send over socket
   */
  def protocolSendData(buffer: ByteBuffer, context: TCPNetworkContext): ByteBuffer

  // LifeCycle
  //-------------------

  /**
   * Prepare Socket
   *
   */
  override def lInit = {

    logInfo("Creating Socket")

    // Create Server Socket
    //----------------------------
    /*if (this.direction == AbstractConnector.Direction.Server) {
             this.serverSocket = ServerSocketChannel.open();
        }*/

  }

  /**
   * Start Server Socket Thread
   */
  /*  override def lStart = {

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

  }*/

  override def lStop = {

    // Stop all threads
    this.stopThread = true

    // Stop Server socket
    //------------
    if (this.direction == AbstractConnector.Direction.Server) {

      // Close Selector to stop operations on thread
      if (this.serverSocketSelector != null && this.serverSocketSelector.isOpen) {
        try {
          this.serverSocketSelector.close
        } catch {
          case e: Throwable =>
        }
      }

    } else {

      this.stopThread = true

      try {
        this.clientSocket.close
      } catch {
        case e: Throwable =>
      }

    }

  }

  // Connector Run Method
  //------------------------

  //-- React on common started to signal ready to go
  on("common.started") {

    started.release(Integer.MAX_VALUE)
  }

  //-- React on common failed to signal to make sure no thread is blocking
  on("common.start.failed") {

    started.release(Integer.MAX_VALUE)
  }

  /**
   * Start this connector in Listening mode if in SERVER direction,
   * or tries to connect to  target address if in CLIENT direction
   *
   */
  override def run = {

    // Common
    //---------------

    //-- Prepare Read Buffer
    //----------------

    // Server Mode
    //-------------------
    if (this.direction == AbstractConnector.Direction.Server) {

      @->("server.start")

      logInfo[TCPConnector](s"Starting TCP Connector on  $address:$port (${this.messageType})")

      // Bind
      //--------------
      this.serverSocket = ServerSocketChannel.open();
      this.serverSocket.bind(new InetSocketAddress(address, port))

      
      // Register Selector for all operations
      // !! Selector only works on non blocking sockets
      //----------------------
      this.serverSocketSelector = Selector.open()
      this.serverSocket.configureBlocking(false);
      this.serverSocket.register(this.serverSocketSelector, SelectionKey.OP_ACCEPT)

      //this.serverSocket.setOption(java.net.StandardSocketOptions.SO_RCVBUF, 4096)

      // Loop on Selection and handle actions
      //------------------
      @->("server.started")
      @->("common.started")
      while (!this.stopThread) {

        try {

          // Select blocking, will throw an exception if socket is closed
          logFine[TCPConnector](s"-- Waiting for something to happen on selector thread")
          var selected = this.serverSocketSelector.select

          var keyIterator = this.serverSocketSelector.selectedKeys.iterator;
          while (keyIterator.hasNext) {

            var key = keyIterator.next();
            key match {

              // Accept
              //--------------------
              case key if (key.isValid && key.isAcceptable) => {

                logFine[TCPConnector]("-- Accepting Connection")

                var clientSocket = serverSocket.accept

                // Prepare Network Context
                //----------------------------
                var networkContext = new TCPNetworkContext(clientSocket)
                networkContext.qualifier = s"client@${networkContext.hashCode}"
                clientsContextsMap += (networkContext.toString -> networkContext)

                @->("server.accepted")
                @->("server.accepted", networkContext)

                // Register Socket Channel to selector
                // !! Selector only works on non blocking sockets
                //-----------------
                clientSocket.configureBlocking(false);
                var clientSocketKey = clientSocket.register(this.serverSocketSelector, SelectionKey.OP_READ, SelectionKey.OP_WRITE)

                //-- Register NetworkContext with key
                clientSocketKey.attach(networkContext)

                keyIterator.remove

              }

              // Read: Pass Read datas to underlying implementation
              //-----------------
              case key if (key.isValid && key.isReadable) => {

                @->("server.read")
                keyIterator.remove

                logFine[TCPConnector]("-- Got Read key")

                //-- Take Channel
                var networkContext = key.attachment().asInstanceOf[TCPNetworkContext]
                var socketChannel = networkContext.socket

                var readBuffer = ByteBuffer.allocate(4096) // (Buffer of a page size per default)

                try {
                  // Read Until 0, or < 0 closes the channel
                  //----------
                  //var continue = true;
                  //while (continue) {

                  socketChannel.read(readBuffer) match {

                    // Continue Reading
                    //------------
                    case readbytes if (readbytes > 0) => {

                      @->("server.read.datas")

                      // Pass Datas to underlying protocol
                      readBuffer.flip

                      /*var passedBuffer = ByteBuffer.allocate(readbytes)
                                                passedBuffer.put(readBuffer)
                                                passedBuffer.flip*/
                      //println("-> Din")
                      var passedBuffer = readBuffer

                      //readBuffer.clear
                      protocolReceiveData(passedBuffer, networkContext) match {
                        case Some(messages) =>

                          //-- Get Message Factory
                          var factory = (Message(this.messageType), networkContext[String]("message.type")) match {

                            //-- Context Switched factory
                            case (_, Some(contextFactoryName)) =>

                              Message(contextFactoryName) match {
                                case Some(factory) => factory
                                case None =>
                                  throw new RuntimeException(s"TCP network context is configured with ${contextFactoryName} message type which has no registered factory")

                              }

                            //-- Normal Connector default factory
                            case (Some(factory), None) => factory

                            case _ =>
                              throw new RuntimeException(s"TCP Connector is configured with ${this.messageType} message type which has no registered factory")

                          }

                          //-- Create Messages
                          messages.foreach {
                            m =>

                              //println("[Server] Got message: "+new String(m.asInstanceOf[ByteBuffer].array()))
                            	logInfo[TCPConnector]("[Server] Got message: "+new String(m.asInstanceOf[ByteBuffer].array()))
                              // Create Message
                              var message = factory(m)

                              // Append context
                              message.networkContext = networkContext

                              // Send
                              this.network ! message
                          }

                        // Protocol not ready
                        case None =>

                      }

                      // Clear Buffer for next read
                      readBuffer.flip()
                      // continue = false
                      //readBuffer.clear();
                    }

                    // Nothing to read
                    //----------------
                    case readbytes if (readbytes == 0) => {

                      @->("server.read.nodatas")
                      readBuffer.clear();
                      // continue = false
                    }

                    // Close Client Connection
                    //------------
                    case readbytes if (readbytes < 0) => {

                      @->("server.read.close")

                      logFine[TCPConnector]("-- Closing Connection")

                      networkContext.@->("close")
                      this.clientsContextsMap -= networkContext.toString
                      socketChannel.close();
                      //continue = false
                    }
                  }
                  //}
                } catch {

                  // IN case of IO error, forget about this connection
                  case e: java.io.IOException =>

                    this.clientsContextsMap -= networkContext.toString
                    logFine[TCPConnector]("-- Closing Connection for: "+networkContext.toString)

                    networkContext.@->("close")

                  case e: Throwable => throw e

                } finally {

                  // keyIterator.remove
                }
              }

              // Fall back
              //----------------
              case key => {
                keyIterator.remove
              }

            }
            // EOF Key matched

            logFine("-- EOF Keys Looop")
          } // EOF While has keys

        } catch {
        	
          // Selector has been closed (connector close for example)
          case e: java.nio.channels.ClosedSelectorException =>
          	
            
            //e.printStackTrace()

          case e: Throwable => throw e

        }

      } // EOF Server thread loop

      // Clean
      //----------------
      this.serverSocketSelector = null

      this.serverSocket.close
      this.serverSocket = null

      @->("server.end")

    } // Client Mode
    //------------------------
    else {

      logInfo[TCPConnector](s"Starting TCP Connector as client on $address:$port")

      // Connect
      var addr = new InetSocketAddress(InetAddress.getByName(this.address), this.port)
      try {
        this.clientSocket = SocketChannel.open(addr)
      } catch {
        case e: Throwable =>

          logError[TCPConnector](s"Failed connection to ${addr} and port $port, not resolved: ${addr.isUnresolved()}")

          // Make sure started signal has been given
          @->("common.start.failed")

          throw e
      }
      // Record Network Context
      this.clientNetworkContext = new TCPNetworkContext(this.clientSocket)
      // this.clientNetworkContext.qualifier = s"server@${this.clientNetworkContext.hashCode}"

      logInfo[TCPConnector](s"Client Started")

      @->("client.started")
      @->("common.started")

      // Data Loop
      //------------------------
      var readBuffer = ByteBuffer.allocate(4096) // (Buffer of a page size per default)
      var continue = true;
      while (continue && !this.stopThread)
        try {
          this.clientSocket.read(readBuffer) match {

            // Continue Reading
            //------------
            case readbytes if (readbytes > 0) => {

              // Pass Datas to underlying protocol
              readBuffer.flip

              /* var passedBuffer = ByteBuffer.allocate(readbytes)
              passedBuffer.put(readBuffer)
              passedBuffer.rewind*/

              protocolReceiveData(readBuffer, clientNetworkContext) match {
                case Some(messages) =>

                  // Get Message Factory
                  Message(this.messageType) match {

                    case Some(factory) =>

                      messages.foreach {
                        m =>

                          logInfo[TCPConnector]("[Client] Got message: " + new String(m.asInstanceOf[ByteBuffer].array()))

                          // Create Message
                          var message = factory(m)

                          // Append context
                          message.networkContext = clientNetworkContext

                          // Send
                          this.network ! message
                      }

                    case None =>
                      throw new RuntimeException(s"TCP Connector is configured with ${this.messageType} message type which has no registered factory")

                  }

                // Protocol not ready
                case None =>

              }

              // protocolReceiveData(passedBuffer, this.clientNetworkContext)

              // Clear Buffer for next read
              readBuffer.flip();
            }

            // Close Client Connection
            //------------
            case readbytes if (readbytes < 0) => {
              this.clientSocket.close();
              continue = false
            }
          }

        } catch {

          // In case of I/O Exception, stop
          case e: java.io.IOException =>
            continue = false

          // Otherwise let live
          case e: Throwable =>
            continue = false
        }

      // EOF Data -> Clean
      //------------------
      this.clientNetworkContext = null
      this.clientSocket = null

    }
  }

}

/**
 * This class is an implementation of TCPConnector, handing out application protocol management to a Protocolhandler class
 *
 */
abstract class TCPProtocolHandlerConnector[T](var protocolHandlerFactory: (TCPNetworkContext => ProtocolHandler[T])) extends TCPConnector {

  // Protocol Implementation
  //----------------
  def protocolReceiveData(buffer: ByteBuffer, context: TCPNetworkContext): Option[Iterable[Any]] = {

    // Receive through Protocol handler
    //---------------
    var handler = ProtocolHandler(context, protocolHandlerFactory)
    handler.receive(buffer)
    handler.availableDatas.size match {
      case size if (size > 0) =>

        var res = List[T]()
        handler.availableDatas.foreach {
          data => res = res :+ data
        }

        @->("protocol.receive.endOfData")
        handler.availableDatas.clear

        Option(res)

      case _ =>

        None
    }

  }

  def protocolSendData(buffer: ByteBuffer, context: TCPNetworkContext): ByteBuffer = {

    // Send though protocol handler
    //-------------------------
    var sendBuffer = ProtocolHandler(context, protocolHandlerFactory).send(buffer, context)

    // Ensure next user can read from content
    //---------
    // println(s"""In protocol handler send, result is ${sendBuffer.remaining} of remaining""")
    sendBuffer.remaining match {
      case 0 =>
        //        println("...so now fliping")
        sendBuffer.flip.asInstanceOf[ByteBuffer]
      case _ =>
        sendBuffer
    }

  }

}

// Network Context
//---------------------
class TCPNetworkContext(q: String) extends NetworkContext {

  this.qualifier = q

  var socket: SocketChannel = null

  def this(so: SocketChannel) = {
    this(so.getRemoteAddress().toString())
    socket = so
    so.getRemoteAddress() match {
      case sa: java.net.InetSocketAddress => this.qualifier = s"${sa.getHostString()}:${sa.getPort()}"
      case _                              =>
    }

  }
}
