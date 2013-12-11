package com.idyria.osi.wsb.core.message.soap

import com.idyria.osi.ooxoo.core.buffers.structural._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._
import java.io.ByteArrayOutputStream
import java.nio.ByteBuffer
import com.idyria.osi.wsb.core.message.MessageFactory
import java.io.ByteArrayInputStream
import com.idyria.osi.ooxoo.lib.json.JsonIO
import java.io.CharArrayReader
import java.nio.CharBuffer
import java.io.StringReader
import com.idyria.osi.ooxoo.lib.json.JsonIO
import java.io.PrintStream
import java.io.CharArrayWriter

/**
 * This is the TOP SOAPMessage class
 *
 * The namespace is SOAP 1.2
 * @xelement(name="Envelope",ns="")
 */

class SOAPMessage extends Envelope with com.idyria.osi.wsb.core.message.Message {

  // Default qualifier
  //------------------
  this.qualifier = "soap"

  // Message Types
  //---------------------
  AnyXList(classOf[Fault])

  // Initialize default Header and Body
  //-------------------
  this.header = new Header
  this.body = new Body

  // Output format
  //-------------------

  /**
   * The used output format.
   * Supported: application/xml and application/json
   */
  var outputFormat = "application/xml"

  def toXMLString = {

    // Create StaxIO
    //-------------------
    var io = new StAXIOBuffer()
    this - io

    // Streamout
    //--------------
    this.streamOut {
      du ⇒
        du("prefixes" -> Map(("http://www.w3.org/2003/05/soap-envelope" -> "env")))
        du
    }

    // Return res
    new String(io.output.asInstanceOf[ByteArrayOutputStream].toByteArray)

  }

  def toBytes = {

    outputFormat match {
      case "application/xml" =>

        // Create StaxIO
        //-------------------
        var io = new StAXIOBuffer()
        this - io

        // Streamout
        //--------------
        this.streamOut {
          du ⇒
            du("prefixes" -> Map(("http://www.w3.org/2003/05/soap-envelope" -> "env")))
            du
        }

        //-- Return res
        ByteBuffer.wrap(io.output.asInstanceOf[ByteArrayOutputStream].toByteArray)

      case "application/json" =>

        // Create JSONIO
        //-------------------
        var out = new CharArrayWriter
        var io = new JsonIO(outputArray =out)
        this - io

        // Streamout
        //--------------
        this.streamOut {
          du ⇒
            du("prefixes" -> Map(("http://www.w3.org/2003/05/soap-envelope" -> "env")))
            du
        }

        //-- Return res
        io.output.flush
        ByteBuffer.wrap(io.finish.getBytes())

      case _ => throw new RuntimeException("Cannot serialize SOAP message to bytes, output format $outputFormat is not supported")
    }

  }

}

class JSONSOAPMessage extends SOAPMessage {

  override def toBytes = {

    // Create JSONIO
    //-------------------
   var out = new CharArrayWriter
        var io = new JsonIO(outputArray =out)
    this - io

    // Streamout
    //--------------
    this.streamOut {
      du ⇒
        du("prefixes" -> Map(("http://www.w3.org/2003/05/soap-envelope" -> "env")))
        du
    }

    //-- Return res
    io.output.flush
    ByteBuffer.wrap(io.finish.getBytes())

  }

}

object JSONSOAPMessage extends MessageFactory {

  def apply(data: Any): JSONSOAPMessage = {

    data match {
      case str: String ⇒

        var msg = new JSONSOAPMessage
        msg - new JsonIO(new StringReader(str))
        msg.lastBuffer.streamIn
        msg

      case byteBuffer: ByteBuffer =>

        println(s"Building json soap from: "+new String(byteBuffer.array().map(_.toChar)))
        
        var msg = new JSONSOAPMessage
        msg - new JsonIO(new CharArrayReader(byteBuffer.array().map(_.toChar)))
        msg.lastBuffer.streamIn
        msg

      case _ ⇒ throw new RuntimeException(s"Cannot not build JSon SOAP Message from unsupported data source: " + data.getClass)
    }

  }

  def apply() = new JSONSOAPMessage

}

/**
 * SOAP Message Factory supports ByteBuffer data
 */
object SOAPMessage extends MessageFactory {

  def apply(data: Any): SOAPMessage = {

    data match {
      case buffer: ByteBuffer ⇒

        var msg = new SOAPMessage
        msg - new StAXIOBuffer(new ByteArrayInputStream(buffer.array()))
        msg.lastBuffer.streamIn
        msg
      case _ ⇒ throw new RuntimeException(s"Cannot not build SOAP Message from Something else than a ByteBuffer ")
    }

  }

  def apply() = new SOAPMessage

}
