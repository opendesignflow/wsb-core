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
/**
 *
 */
package com.idyria.osi.wsb.core.network

import com.idyria.osi.wsb.core._
import com.idyria.osi.wsb.core.message._
import com.idyria.osi.wsb.core.network.connectors.AbstractConnector
import com.idyria.osi.wsb.core.network.connectors.ConnectorFactory
import com.idyria.osi.tea.logging.TLogSource
import com.idyria.osi.wsb.core.network.dispatch.Dispatch
import com.idyria.osi.wsb.core.network.dispatch.ExecutorDispatch

/**
 * @author rleys
 *
 */
class Network(var engine: WSBEngine) extends Lifecycle with TLogSource {

  var connectors = Set[AbstractConnector[_]]()

  // Engine Connection
  //-------------

  /**
   * Method to send an event to the local Engine internal Bus
   */
 // def !(msg: AnyRef) = this.engine ! msg

  //  Message Send path
  //---------------

  /**
   *
   * Tries to send a message through one of the avaible connectors
   *
   * If no connector has an existing network context matching the message's,
   * then a connector may try to handle it as a new connection to the outside world
   *
   */
  def send(msg: Message): Unit = {

    // Try to find a connector that will handle the message
    //----------------------
    connectors.find { c => c.canHandle(msg) } match {

      // SERVER: Connector Exists
      //-----------------------------------
      case Some(connector) =>

        logFine[Network]("Found Connector to send message: "+connector)
        try {
          connector.send(msg)
        } catch {
          case e: Throwable =>
            msg(e)
            e.printStackTrace()
        }

      // CLIENT MODE: Try to create a connector for the message in client mode
      //----------------------
      case None if (msg.networkContext.isDefined) =>

        // IF the context string does not match client format, this is normal, and will throw an exception
        try {
          ConnectorFactory(msg.networkContext.get.qualifier) match {

            case Some(connector) =>

              logFine(s"------ Creating CLIENT connector")

              // Register and Start
              //-------------------
              this.addConnector(connector)
              connector.direction = AbstractConnector.Direction.Client
              connector.cycleToStart
              connector.waitForStart()

              // Send
              //-------------

              connector.send(msg)

            case _ => throw new RuntimeException("Cannot send message because no Connector would send it ")
          }
        } catch {
          case e: Throwable =>
            msg(e)
            e.printStackTrace()
        }

      // Client: Message without Network Content
      //----------------------
      case None => 
        throw new RuntimeException("Cannot send message because no network context was provided")
        
    }

    //

  }

  // Connectors Management
  //--------------------
  def addConnector[T <: AbstractConnector[_]](connector: T) = {

    connector.network = this
    connectors += connector

  }
  
  // Dispatch
  //------------
  
  var dispatchImplementation : Option[Dispatch] = Some(new ExecutorDispatch)
  
  
  def getDispatch = dispatchImplementation match {
    case Some(d) => d 
    case None => throw new RuntimeException("No dispatch has been defined on Network")
  }
  
  def dispatch(m:Message) = {
    
    getDispatch.deliver(m,this.engine.broker)
  }

  // Lifecycle
  //---------------------

  def lInit = {

    // Init Connectors
    this.connectors.foreach(_.cycleToInit)

  }

  /**
   * Start WSBEngine
   */
  def lStart = {

    // Start connectors
    this.connectors.foreach(_.cycleToStart)

  }

  def lStop = {

    // Stop connectors
    try {
      this.connectors.foreach(_.cycleToStop)
    } catch {
      case  e : Throwable => 
        e.printStackTrace()
    }
    
    // Stop dispatch
    this.dispatchImplementation match {
      case Some(dispatch) => 
        dispatch.lstop
      case None => 
    }
    

  }

}
object Network {

}
