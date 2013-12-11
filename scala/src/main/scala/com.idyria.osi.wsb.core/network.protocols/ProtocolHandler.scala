package com.idyria.osi.wsb.core.network.protocols


import  com.idyria.osi.wsb.core.network._
import java.nio.channels._
import java.nio._
import scala.collection.mutable._
import com.idyria.osi.wsb.core.message.Message


 /**
    This class is created per client to handle the datas and decode protocol
*/
abstract class ProtocolHandler[DT <: Any](var context : NetworkContext)  {


    /**
        This stack holds the data that are available from receive
    */
    var availableDatas = new ArrayStack[DT]()

    /**

        @return true if receiving is completed, and some data is available, false otherwise
    */
    def receive(buffer : ByteBuffer) : Boolean

    def send(buffer: ByteBuffer,context:NetworkContext) : ByteBuffer
    
}

// Companion object to get Protocol Handler from Network Context
object ProtocolHandler {

    def apply[T <: NetworkContext,DT]( context : T, factory: (T => ProtocolHandler[DT])) :ProtocolHandler[DT] = {

        // Create Procotol handler if non existent
        //----------------
        if(!context.attachments.contains("protocol.handler")) {
            context.attachments += ("protocol.handler" -> factory(context) )
        
            //println("Added protocol handler instance to context: "+context)
        }

        // Return
        context.attachments("protocol.handler").asInstanceOf[ProtocolHandler[DT]]



  }

  def apply[T <: NetworkContext,DT]( context : T) :ProtocolHandler[DT] = {


        println("Getting protocol handler instance from context: "+context)

        // Return null Procotol handler if non existent
        //----------------
        if(!context.attachments.contains("protocol.handler")) {
           return null
        }

        // Return
        context.attachments("protocol.handler").asInstanceOf[ProtocolHandler[DT]]

  }

}


class ProtocolHandlerException(var message : String) extends Exception(message) {



}
