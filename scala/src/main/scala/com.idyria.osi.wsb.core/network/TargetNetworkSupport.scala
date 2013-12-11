package com.idyria.osi.wsb.core.network



/**
 * Trait providing saving facility for classes that would likely send results to multiple network contexes
 */
trait TargetNetworkSupport {
  
  
  var targetNetwork = List[NetworkContext]()
  
  /**
   * Record a network context
   */
  def apply(nc:NetworkContext) = {
    
    // Register
    targetNetwork.synchronized {
    	
      targetNetwork = targetNetwork :+ nc
    }
    
    // Listen on close to avoid invalid sends
    nc.on("close") {
      println("-- Removing from TargetNetwork Support")
      targetNetwork.synchronized {
        targetNetwork = targetNetwork.filterNot(_==nc)
      }
      
    }
  }
  
  
  /**
   * Apply a closure for each configured network contexes
   */
  def onEachTargetNetwork(cl: NetworkContext => Any) : Unit = {
    targetNetwork.synchronized {
    	targetNetwork.foreach(cl(_))
    }
    
  }
  
  
  
}