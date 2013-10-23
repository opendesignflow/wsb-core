package com.idyria.osi.wsb.core.message.soap

import com.idyria.osi.ooxoo.core.buffers.structural._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._
import java.io.ByteArrayOutputStream
import java.nio.ByteBuffer
import com.idyria.osi.wsb.core.message.MessageFactory
import java.io.ByteArrayInputStream

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

  }

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
