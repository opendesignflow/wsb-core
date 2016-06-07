package com.idyria.osi.wsb.core.network.dispatch

import com.idyria.osi.wsb.core.broker.MessageBroker
import com.idyria.osi.wsb.core.message.Message

trait Dispatch {
  
  /**
   * Ensures the Provided message is passed to the message broker
   */
  def deliver(m:Message,b:MessageBroker)  : Unit
}