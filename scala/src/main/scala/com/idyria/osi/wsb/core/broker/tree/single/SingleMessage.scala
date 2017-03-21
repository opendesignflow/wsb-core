package com.idyria.osi.wsb.core.broker.tree.single

import com.idyria.osi.wsb.core.message.Message

trait SingleMessage extends Message {
  
  var virtualChannel : Option[String] = None
  
  def getVirtualChannelAsString = virtualChannel match {
    case Some(vc) => vc
    case None => ""
  }
}