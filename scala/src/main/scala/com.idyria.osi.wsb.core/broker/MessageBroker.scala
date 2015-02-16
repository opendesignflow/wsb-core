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
package com.idyria.osi.wsb.core.broker

import com.idyria.osi.wsb.core._
import com.idyria.osi.wsb.core.broker.tree._
import com.idyria.osi.wsb.core.message.Message
import com.idyria.osi.wsb.core.message.UpMessage
import com.idyria.osi.tea.logging.TLogSource

  
class MessageBroker ( var engine : WSBEngine ) extends Lifecycle with TLogSource {

   
 
  // Brokering Tree
  //---------------------

  /**
   * The default brokering tree used. can be easily improved by API
   */
  var brokeringTree = new Intermediary {


    /**
      Send message through network
    */
    upClosure = {
      message => 

        engine.network.send(message)

    }

  }

   
  val messageReceiveClosure = {

      msg : Message => 

        msg match {
          case m : UpMessage => 
            
            logFine[MessageBroker]("received Message to up") 
            
            brokeringTree.up(msg)
            
          case _ => 
            
            logFine[MessageBroker]("Broker Got Down Message: "+msg.getClass) 
            logFine[MessageBroker]("-> Qualifier "+msg.qualifier) 
            brokeringTree.down(msg) 
        }
        
          

          // broker
          

  }

  /**
    Add an intermediary to the root  Intermediary
  */
  def <=(intermediary: Intermediary) = brokeringTree.<=(intermediary)
  
  def prepend(intermediary: Intermediary) = brokeringTree.prepend(intermediary)

  def -=(intermediary: Intermediary) = brokeringTree.-=(intermediary)

  // Engine Connection
  //-------------

  //-- Listen for Messages to be handled

  /**
    Method to send an event to the local Engine internal Bus
  */
  def ! ( msg :  AnyRef) = this.engine ! msg


  // Lifecycle
  //------------------------

  /**
    Init Message Broker

  */
  def lInit = {

    // Register Message Closure to aib
    engine.localBus.registerClosure(messageReceiveClosure)


 
  }

  /**
   * Start Message Broker
   */
  def lStart = {

    // Nothing to do

  }

  def lStop = {

    // Nothing to do

  }




}
object MessageBroker {



}
