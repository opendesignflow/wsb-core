/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2014 OSI / Computer Architecture Group @ Uni. Heidelberg
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/gpl-3.0.html>.
 * #L%
 */
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
