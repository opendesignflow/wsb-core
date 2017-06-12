/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2017 OpenDesignFlow.org
								Richard Leys (leys dot richard at gmail):
								2008-2014 University of Heidelberg (Computer Architecture group)
								2008-2014 Extoll GmbH (extoll.de)
								2014-2017 University of Karlsruhe (KIT) - ASIC and Detector Lab
 * %%
 * This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    var availableDatas = new scala.collection.mutable.ArrayBuffer[DT]()

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
      context.attachments.get("protocol.handler") match {
        case Some(handler) => 
          //println(s"Found a Protocol handler on context: "+handler.getClass().getCanonicalName)
          handler.asInstanceOf[ProtocolHandler[DT]]
        case None => 
          var newHandler = factory(context)
           context.attachments += ("protocol.handler" -> newHandler )
           newHandler
          
      }
     



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
