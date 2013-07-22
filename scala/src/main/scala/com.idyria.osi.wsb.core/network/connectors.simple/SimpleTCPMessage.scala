package com.idyria.osi.wsb.core.network.connectors.simple


import com.idyria.osi.wsb.core.network._
import com.idyria.osi.wsb.core.network.connectors.tcp._

import com.idyria.osi.wsb.core.network.protocols._


import java.nio.channels._
import java.nio._

/**
    This Connector provides a TCP connector with following protocol:

    Content-Length: (length in bytes)\n
    message of "length bytes"\n


    Simple as hell :=)

*/
class SimpleTCPMessage extends TCPProtocolHandlerConnector( ctx => new SimpleProtocolHandler(ctx) ) {




}


/**
    This class is created per client to handle the datas and decode protocol
*/
class SimpleProtocolHandler(var localContext : NetworkContext) extends ProtocolHandler(localContext) {


    def receive(buffer : ByteBuffer) = {

    }

    def send (buffer: ByteBuffer) :  ByteBuffer  = {
        null
    }

}
