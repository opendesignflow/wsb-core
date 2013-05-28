/**
 *
 */
package com.idyria.osi.wsb.core.network.connectors

import java.net.ServerSocket
import java.net.Socket
import scala.io.Source
import com.idyria.osi.wsb.core.message.HTTPMessage
import com.idyria.osi.wsb.core.network.AbstractConnector
import javax.net.ServerSocketFactory
import java.io.PrintStream
import com.idyria.osi.wsb.core.broker.MessageBroker

/**
 * @author rleys
 *
 */
class HTTPConnector( var port : Int ) extends AbstractConnector {

  /**
   * Connection address
   */
  var address = "localhost"
  
  var socket : ServerSocket = null  
  
  /**
   * Maps a string to te client handler, for backpath matching
   */
  var clientSocketsMap =  scala.collection.mutable.Map[String,ClientHandler]()
  
  
  /**
   * Handles a client Socket
   * 
   * HTTP: Empty line define end of request.
   * FIXME: The end of line may not come in one getLines call
   * 
   */
  class ClientHandler(var clientSocket : Socket) extends Thread {
    
    this.setDaemon(true)
    
    override def run = {
      
      // Take Socket stream
      var socketSource = Source.fromInputStream(clientSocket.getInputStream(), "UTF-8")
      
      // Register in Socket Map
      clientSocketsMap += (this.clientSocket.getRemoteSocketAddress().toString() -> this)
      
      logInfo ("Starting Client Handler")
      
      var stop = false
      while(!stop) {
        
        // Take Lines and create message
        var lines = scala.collection.mutable.ArrayBuffer[String]()
        var lastLine = false;
        try {
	        do {
	           socketSource.getLines.foreach {
	               line =>
		               // If last line, create message and dispatch to handler
		               if (line.equals("")) {
		                 //lastLine = true
		                 //writeMessage(null)
		                 //stop = true
		                 
		                 // Parse Message
		                 var message = HTTPMessage.build(lines.toList)
		                 MessageBroker.handleMessage(message)
		                 
		               } else {
		                 lines += line
		               }
	           }
	          
	           
	        } while(!lastLine)
        } catch {
          
          case e : Throwable => stop = true;
        }
        //var message = HTTPMessage.build(lines.toList)
        
        // Send Message to handler
        
        // Return Dummy Stuff
        
        
      }
      
    }
    
    def writeMessage(message : HTTPMessage) = {
      
      logInfo("Sending Dummy HTML")
      
      // Prepare Writer
      var printStream = new PrintStream(this.clientSocket.getOutputStream())
      
      var page = s"""
      <html>
      	<head>
      
      	</head>
      	<body>
      		<h1>Hello World!!</h1>
      	</body>
      </html>
      """
      
      printStream.println("HTTP/1.1 200")
      printStream.println("Content-Type: text/html")
      printStream.println("Content-Length: "+page.getBytes().length+"")
      printStream.println(page)
      printStream.println();
      printStream.flush()
      this.clientSocket.close()
      
    }
    
  }
  
  
  override def lInit = {
    
    logInfo ("Creating Socket")
    
    // Create Server Socket
    this.socket = ServerSocketFactory.getDefault().createServerSocket(port)
    
    
  }
  
  override def run = {
    
    logInfo("Starting HTTP Connector")
    
    while(!this.stopThread) {
    
	    // Listen
	    //--------------
	    
	    // Accept Connections
	    //---------------------------
    	try {
	    var clientSocket = this.socket.accept();
	    if (clientSocket==null) {
	    	
	      //-- Stop if connection broken
	      this.stopThread = true
	      
	    } else {
	      
	      //-- Launch handler thread and loop
	      logInfo("Handling Connection")
	      var clientHandler = new ClientHandler(clientSocket)
	      clientHandler.start();
	      
	    }
	    } catch {
	      case e : Throwable => {
	        this.stopThread = true
	      }
	    }
    }
    
  }
  
  def lStop = {
    
    this.stopThread=true
    try {
    	this.socket.close()
    } catch {
      case e : Throwable => {
        
      }
    }
    
  }
  
  
}