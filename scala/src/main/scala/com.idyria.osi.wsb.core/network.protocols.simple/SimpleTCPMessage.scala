package com.idyria.osi.wsb.core.network.connectors.simple


import com.idyria.osi.wsb.core.network._
import com.idyria.osi.wsb.core.network.connectors.tcp._
import com.idyria.osi.wsb.core.network.protocols._

import com.idyria.osi.tea.listeners.ListeningSupport

import java.nio.channels._
import java.nio._

/**
    This Connector provides a TCP connector with following protocol:

    Content-Length: (length in bytes)\n
    message of "length bytes"\n


    Simple as hell :=)

*/
class SimpleMessageTCPConnector extends TCPProtocolHandlerConnector[ByteBuffer]( ctx => new SimpleProtocolHandler(ctx) ) {




}

object SimpleMessageTCPConnector {


    //implicit def convertStringToByteBuffer(str : String ) : ByteBuffer = ByteBuffer.wrap(str.getBytes)

}


/**
    This class is created per client to handle the datas and decode protocol

    Content-Length: (length in bytes)\n
    message of "length bytes"

*/
class SimpleProtocolHandler(var localContext : NetworkContext) extends ProtocolHandler[ByteBuffer](localContext) with ListeningSupport {


    var receiveContentLengthBuffer =  CharBuffer.allocate(1024)

    var receiveContentBuffer : ByteBuffer =  null

    var receiveSearchContentLength = true

    def receive(buffer : ByteBuffer) : Boolean = {

        var newDataAvailable = false

        // Global Loop until input buffer is empty
        //-----------------------------
        while (buffer.remaining>0) {

            @->("start",buffer)

            // State1. Search for ContentLength
            //  - Buffer until \n is found
            //----------
            while(receiveSearchContentLength && buffer.remaining>0) {


               // println("Content length char analysis")

                buffer.get match {

                    // \n found , allocate buffer
                    case char if (char=='\n') => {

                        // Get content length and reset
                        this.receiveSearchContentLength = false

                        var contentLengthString = this.receiveContentLengthBuffer.clear.toString
                        this.receiveContentLengthBuffer.clear

                        @->("contentLength.foundFirstLine",contentLengthString)

                        // Parse
                        var parseExpression = """Content-Length:\s*([0-9]+)""".r
                        parseExpression.findFirstMatchIn(contentLengthString) match {

                            // Get Length and allocate buffer
                            case Some(res) => {

                                var length= Integer.parseInt(res.group(1))
                                this.receiveContentBuffer = ByteBuffer.allocate(length)
                            }
                            case None =>
                                throw new ProtocolHandlerException(s"First line of data must respect the format: ${parseExpression.toString}, found: $contentLengthString")
                        }


                    }

                    // Not found
                    case char =>

                        //println("Buffering for content length")

                        @->("contentLength.buffering",char)
                        this.receiveContentLengthBuffer.put(char.asInstanceOf[Char])
                }

            }

            // Receive normal content
            //----------------
            if (buffer.remaining>0) {

                // Receive only until receiveContentBuffer is full
                //-------------
                var toReceive = (buffer.remaining > this.receiveContentBuffer.remaining()) match {
                                        case true =>  this.receiveContentBuffer.remaining()
                                        case false => buffer.remaining()
                }

                // Receive
                //---------------
                try {
                    this.receiveContentBuffer.put(buffer)
                }
            }

            // If receive buffer is full then we got the message
            //---------------------------------------
            if (this.receiveContentBuffer!=null && this.receiveContentBuffer.remaining==0) {

                // Add to data stack
                this.availableDatas += this.receiveContentBuffer


                // Reset
                this.receiveContentBuffer = null
                this.receiveSearchContentLength = true
                newDataAvailable = true

            }

        }

        // Data avaiable ?
        newDataAvailable
    }

    /**
        The Send Method must provide a full Buffer
        Each call to send() creates a message

    */
    def send (buffer: ByteBuffer) :  ByteBuffer  = {



        // If Start of send, send content line
        //--------------
        var contentLine = s"""Content-Length: ${buffer.capacity}\n"""
        var contentLineBytes = contentLine.getBytes

        // Output
        //-------------
        var res = ByteBuffer.allocate(contentLineBytes.length+buffer.capacity)
        res.put(ByteBuffer.wrap(contentLineBytes))
        res.put(buffer)

        // Return
        //-------------
        return res


    }

}
