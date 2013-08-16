package com.idyria.osi.wsb.core.message.soap

import com.idyria.osi.ooxoo.core.buffers.structural._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._







/**
    This is the TOP SOAPMessage class

    The namespace is SOAP 1.2

*/
@xelement(name="Envelope",ns="http://www.w3.org/2003/05/soap-envelope")
class SOAPMessage extends ElementBuffer {

    // Elements
    //--------------------
    @xelement(name="Header",ns="http://www.w3.org/2003/05/soap-envelope")
    class Header extends ElementBuffer {

        @any
        var content = AnyXList()


    }

    @xelement(name="Body",ns="http://www.w3.org/2003/05/soap-envelope")
    class Body  extends ElementBuffer {

        @any
        var content = AnyXList()

    }
    

    // Structure
    //----------------

    @xelement(name="Header",ns="http://www.w3.org/2003/05/soap-envelope")
    var header = new Header

    @xelement(name="Body",ns="http://www.w3.org/2003/05/soap-envelope")
    var body = new Body


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
