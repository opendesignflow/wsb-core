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

  var direction = ""
  
  /**
   * Set to true on sto, implementin Thread must stop when this is set to true
   */
  var stopThread = false
  
    
  /**
   * Start a connector by starting a thrad
   */
  def lStart = {
    
    this.start()
    
    
  }
  
  /**
   * To be implemented by Server connector
   */
  override def run
    
  
}