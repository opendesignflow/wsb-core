/**
 *
 */
package com.idyria.osi.wsb.core.network

import com.idyria.osi.wsb.core.Lifecycle

/**
 * @author rleys
 *
 */
class Network {

}
object Network extends Lifecycle {
  
  
  var connectors = scala.collection.mutable.Set[AbstractConnector]()
  
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