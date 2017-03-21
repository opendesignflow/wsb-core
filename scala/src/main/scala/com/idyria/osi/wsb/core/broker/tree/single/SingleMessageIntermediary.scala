package com.idyria.osi.wsb.core.broker.tree.single

import com.idyria.osi.wsb.core.message.Message
import java.util.concurrent.locks.ReentrantLock
import scala.reflect.ClassTag
import com.idyria.osi.wsb.core.broker.tree.MessageIntermediary

class SingleMessageIntermediary[MT <: SingleMessage](implicit val ttag: ClassTag[MT]) extends MessageIntermediary[MT] {

  println(s"Creating Single Message intermediary from: " + messageType)

  /**
   * Busy Locks are store in a map pro virtual Channel
   */
  var busyLocks = Map[String, ReentrantLock]()

  //var busyLock = new ReentrantLock(false)

  /**
   * Override catcher to throttle receives
   * Last in Train messages are waiting extra
   */
  override def onDownMessage(cl: MT => Unit) = {
    super.onDownMessage {
      message =>

        // Get Lock
        //-----------------
        var busyLock = busyLocks.synchronized {
          busyLocks.get(message.getVirtualChannelAsString) match {
            case Some(lock) => lock
            case None =>
              var lock = new ReentrantLock(false)
              this.busyLocks = this.busyLocks + (message.getVirtualChannelAsString -> lock)
              lock
          }
        }

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