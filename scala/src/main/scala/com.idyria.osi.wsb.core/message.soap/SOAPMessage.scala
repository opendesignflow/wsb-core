package com.idyria.osi.wsb.core.message.soap

import com.idyria.osi.ooxoo.core.buffers.structural._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._


import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._

 


/**
    This is the TOP SOAPMessage class

    The namespace is SOAP 1.2
    @xelement(name="Envelope",ns="")
*/

class SOAPMessage extends Envelope {

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
            du => 
                du("prefixes" -> Map( ("http://www.w3.org/2003/05/soap-envelope" -> "env")  ))
                du
        }

        // Return res
        new String(io.output.toByteArray)

    }

}

object SOAPMessage {

    def apply() = new SOAPMessage
}
