/**
 *
 */
package com.idyria.osi.wsb.core.network

import com.idyria.osi.wsb.core._
import com.idyria.osi.wsb.core.message._
import com.idyria.osi.wsb.core.network.connectors.AbstractConnector
import com.idyria.osi.wsb.core.network.connectors.ConnectorFactory

/**
 * @author rleys
 *
 */
class Network(var engine: WSBEngine) extends Lifecycle {

  var connectors = Set[AbstractConnector[_]]()

  // Engine Connection
  //-------------

  /**
   * Method to send an event to the local Engine internal Bus
   */
  def !(msg: AnyRef) = this.engine ! msg

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

      // 
      case Some(connector) =>

        try {
          connector.send(msg)
        } catch {
          case e: Throwable =>
            msg(e)
            e.printStackTrace()
        }
      case None =>

        // Try to create a connector for the message in client mode
        //----------------------
        
        // IF the context string does not match client format, this is normal, and will throw an exception
        try {
          ConnectorFactory(msg.networkContext.qualifier) match {

            case Some(connector) =>

              // Register and Start
              //-------------------
              this.addConnector(connector)
              connector.direction = AbstractConnector.Direction.Client
              connector.lStart
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

    }

    //

  }

  // Connectors Management
  //--------------------
  def addConnector[T <: AbstractConnector[_]](connector: T) = {

    connector.network = this
    connectors += connector

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
    this.connectors.foreach(_.cycleToStop)

  }

}
object Network {

}
