package com.idyria.osi.wsb.core.broker.tree.single

import com.idyria.osi.wsb.core.message.Message
import java.nio.ByteBuffer

trait SingleMessage extends Message {
  
  var virtualChannel : Option[String] = None
  
  def getVirtualChannelAsString = virtualChannel match {
    case Some(vc) => vc
    case None => ""
  }
}

class DefaultSingleMessage extends SingleMessage {
  
  def toBytes = ByteBuffer.allocate(0)
}