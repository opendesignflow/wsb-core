/**
 *
 */
package com.idyria.osi.wsb.core.network

import com.idyria.osi.wsb.core._

/**
 * @author rleys
 *
 */
class Network ( var engine : WSBEngine ) extends Lifecycle {

  var connectors = Set[AbstractConnector[NetworkContext]]()

  // Engine Connection
  //-------------

  /**
    Method to send an event to the local Engine internal Bus
  */
  def ! ( msg :  AnyRef) = this.engine ! msg


  // Connectors Management
  //--------------------
  def addConnector ( connector : AbstractConnector[NetworkContext]) = {

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
object Network  {





}
