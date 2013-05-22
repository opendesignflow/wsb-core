/**
 *
 */
package com.idyria.osi.wsb.core

import com.idyria.osi.wsb.core.network.Network

/**
 * @author rleys
 *
 */
class WSBEngine {

}
object WSBEngine extends Lifecycle {
  
  def lInit = {
    
    // Init Network
    Network.cycleToInit
    
  }
  
  /**
   * Start WSBEngine
   */
  def lStart = {
    
    // Start network
    Network.cycleToStart
    
  }
  
  def lStop = {
    
    // Stop Connector
    Network.cycleToStop
    
  }
  
}