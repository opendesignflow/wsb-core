package com.idyria.osi.wsb.core.network.connectors.http

import com.idyria.osi.wsb.core.network._
import com.idyria.osi.wsb.core.network.connectors._
import com.idyria.osi.wsb.core.network.connectors.tcp._
import com.idyria.osi.wsb.core.network.protocols._

import com.idyria.osi.wsb.core.message._
import com.idyria.osi.wsb.core.message.http._

import com.idyria.osi.tea.listeners.ListeningSupport

import java.nio.channels._
import java.nio._
import java.io._


import scala.io.Source

class HTTPConnector2( cport : Int) extends TCPProtocolHandlerConnector[scala.collection.mutable.ArrayBuffer[String]]( ctx => new HTTPProtocolHandler(ctx) ) {


	this.port = cport
	this.messageType = "http"

	Message("http",HTTPMessage)
}

object HTTPConnector2 {

	def apply(port: Int) : HTTPConnector2 = new HTTPConnector2(port)
}

class HTTPProtocolHandler (var localContext : NetworkContext) extends ProtocolHandler[scala.collection.mutable.ArrayBuffer[String]](localContext) with ListeningSupport {

	// Receive
	//-----------------
	var lastLine = false;
	
	// Take Lines and create message
    var lines = scala.collection.mutable.ArrayBuffer[String]()

	// Send
	//---------------------

	/**
		REceive some HTTP
	*/
	def receive(buffer : ByteBuffer) : Boolean = {

		@->("http.connector.receive",buffer)
		println("Got HTTP Datas: "+new String(buffer.array))
    	
    	// Use SOurce to read from buffer
    	//--------------------
    	var bytesSource = Source.fromInputStream(new ByteArrayInputStream(buffer.array))

       bytesSource.getLines.foreach {
           line =>
               // If last line, create message and dispatch to handler
               if (line.equals("")) {
                 lastLine = true
                 //writeMessage(null)
                 //stop = true

                 // Parse Message
                 //var message = HTTPMessage.build(lines.toList)
                 //HTTPConnector.this.network ! message

                 println("---> Reached last line: ")

               } else {
                 lines += line
               }
       }

       // Reset if at the end of Request
       lastLine match {
       		case true => 
       			lastLine = false

       			this.availableDatas += lines 
       			this.lines = scala.collection.mutable.ArrayBuffer[String]()

       			true
       		case false => false
       }


	}

	def send (buffer: ByteBuffer) :  ByteBuffer  = {
		buffer
	}

}