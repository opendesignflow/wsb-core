/**
 *
 */
package com.idyria.osi.wsb.core

import com.idyria.osi.wsb.core.network.Network
import com.idyria.osi.wsb.core.broker.MessageBroker

import com.idyria.osi.aib.core.bus.aib

/**
 * @author rleys
 *
 */
class WSBEngine  extends Lifecycle {


  // Local Aib Bus
  //-------------------------
  var localBus = new aib

  /**
    Method to send an event to the local aib Bus
  */
  def ! ( msg :  AnyRef) = this.localBus send msg

  // Layers
  //----------------

  var network = new Network(this)

  var broker = new MessageBroker(this)


  /**
    Init Sub services
  */
  def lInit = {

    Set(network,broker).foreach(_.cycleToInit)

  }

  /**
    Start Sub services
  */
  def lStart = {

   Set(network,broker).foreach(_.cycleToStart)

  }

  /**
    Stop Sub services
  */
  def lStop = {

    Set(network,broker).foreach(_.cycleToStop)
  }

  /**
    Also clean internal aib
  */
  override protected def lClean = {

    this.localBus.doStop

  }

}
object WSBEngine {



}
