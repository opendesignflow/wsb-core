/**
 *
 */
package com.idyria.osi.wsb.core.network

import com.idyria.osi.wsb.core.Lifecycle
import com.idyria.osi.wsb.core.Logsource

import java.util.concurrent._

import java.nio._

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
    Semaphore used by implementatino to signal it is ready to operate
  */
  var started = new Semaphore(0);


  // User Interface
  //----------------------

  /**
    Used by clients to send datas
  */
  def send(data: ByteBuffer)

  // Lifecycle
  //-----------------

  /**
   * Start a connector by starting a thread
   */
  def lStart = {

    this.start()


  }

  // Run
  //----------------

  /**
   * To be implemented by Server connector
   */
  override def run


}

/**

  This Companion object contains some utility conversions

*/
object AbstractConnector {

   object Direction extends Enumeration {
      type Direction = Value
      val Client , Server = Value
   }


   implicit def convertStringToByteBuffer(str : String ) : java.nio.ByteBuffer = java.nio.ByteBuffer.wrap(str.getBytes)

}
