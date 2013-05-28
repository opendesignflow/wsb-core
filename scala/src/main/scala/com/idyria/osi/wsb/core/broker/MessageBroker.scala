package com.idyria.osi.wsb.core.broker

import com.idyria.osi.wsb.core.message.Message
import com.idyria.osi.wsb.core.broker.tree.BrokeringTree

class MessageBroker {

  
  
  
}
object MessageBroker {
  
  /**
   * The default brokering tree used. can be easily improved by API
   */
  var brokeringTree = new BrokeringTree()
  
  def handleMessage(message:Message) = {
    
  }
  
  def down(message:Message) = {
    
  }
  
}