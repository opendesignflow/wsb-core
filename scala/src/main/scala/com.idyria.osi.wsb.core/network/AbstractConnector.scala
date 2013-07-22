/**
 *
 */
package com.idyria.osi.wsb.core.network

import com.idyria.osi.wsb.core.Lifecycle
import com.idyria.osi.wsb.core.Logsource

/**
 * @author rleys
 *
 */
abstract class AbstractConnector extends Thread with Lifecycle with Logsource {

  /**
    Network This Connector is registered under
  */
  var network : Network = null


  /**
    CLIENT/SERVER Direction
  */
  var direction = AbstractConnector.Direction.Server

  /**
   * Set to true on stop, implementin Thread must stop when this is set to true
   */
  var stopThread = false


  /**
   * Start a connector by starting a thread
   */
  def lStart = {

    this.start()


  }

  /**
   * To be implemented by Server connector
   */
  override def run


}

object AbstractConnector {

   object Direction extends Enumeration {
      type Direction = Value
      val Client , Server = Value
   }


}
