package com.idyria.osi.wsb.core.network.dispatch

import java.util.concurrent.Executors
import java.util.concurrent.ThreadFactory
import com.idyria.osi.wsb.core.broker.tree.BrokeringTree
import com.idyria.osi.wsb.core.message.Message
import com.idyria.osi.wsb.core.broker.MessageBroker

class ExecutorDispatch extends ThreadFactory with Dispatch {
  
  // Thread creations 
  //---------------------

  //-- Thread group
  var threadGroup = new ThreadGroup("wsb.network.dispatch.executor")

  def newThread(r: Runnable) = {

    var th = new Thread(threadGroup, r)

    th
  }
  
  val timedExecutor = Executors.newScheduledThreadPool(8, this)
  
  def deliver(m:Message,b:MessageBroker) : Unit =  {
    
    timedExecutor.submit(new Runnable {
      def run = {
        b.brokeringTree.down(m)
      }
    })
    
  }
  
  def lstop = {
    timedExecutor.shutdownNow()
  }
  
  
}