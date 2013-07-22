package com.idyria.osi.wsb.core.network.protocols


import  com.idyria.osi.wsb.core.network._

import java.nio.channels._
import java.nio._


 /**
    This class is created per client to handle the datas and decode protocol
*/
abstract class ProtocolHandler(var context : NetworkContext) {


    def receive(buffer : ByteBuffer)

    def send (buffer: ByteBuffer) : ByteBuffer

}

// Companion object to get Protocol Handler from Network Context
object ProtocolHandler {

    def apply[T <: NetworkContext]( context : T, factory: (T => ProtocolHandler)) :ProtocolHandler = {

        // Create Procotol handler if non existent
        //----------------
        if(!context.attachments.contains("protocolHandler")) {
            context.attachments += ("protocolHandler" -> factory(context) )
        }

        // Return
        context.attachments("protocolHandler").asInstanceOf[ProtocolHandler]

  }

}
