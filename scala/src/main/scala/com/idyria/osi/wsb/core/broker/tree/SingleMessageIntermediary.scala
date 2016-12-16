package com.idyria.osi.wsb.core.broker.tree

import com.idyria.osi.wsb.core.message.Message
import java.util.concurrent.locks.Lock
import java.util.concurrent.locks.ReentrantLock
import scala.reflect.ClassTag


class SingleMessageIntermediary[MT <: Message] (implicit val ttag: ClassTag[MT])  extends MessageIntermediary[MT] {
 
  println(s"Creating Single Message intermediary from: "+messageType)
  
  var busyLock = new ReentrantLock(false)

  /**
   * Override catcher to throttle receives
   * Last in Train messages are waiting extra
   */
  override def onDownMessage(cl: MT => Unit) = {
    super.onDownMessage {
      message =>
       // println(s"Checking Lock for message: "+message+" -> "+busyLock.tryLock()+" on "+this)
       // busyLock.tryLock()
        busyLock.tryLock() match {
          case true =>
            //println(s"Got Lock for message: "+message+" -> on "+this)
            try {
              cl(message)
            } finally {
              busyLock.unlock()
            }
          case false if (message.lastInTrain) => 
            busyLock.lock()
            try {
              cl(message)
            } finally {
              busyLock.unlock()
            }
          case false =>
        }
    }

  }

}