package com.idyria.osi.wsb.core.message

import com.idyria.osi.ooxoo.core.buffers.structural.io.sax.STAXSyncTrait
import java.nio.ByteBuffer

trait OOXOOMessage extends Message with STAXSyncTrait {
  
  def toBytes = ByteBuffer.wrap(this.toXMLString.getBytes)
}