package com.idyria.osi.wsb.lib.soap

import com.idyria.osi.ooxoo.model.ModelBuilder
import com.idyria.osi.ooxoo.model.Element


/**
 * Provides extra functions to simplify model building
 */
trait ProtocolBuilder extends ModelBuilder {
  
  // Messages Defintions
  //-----------
  implicit class Message(var baseName : String) {
    
  }
  
  var messagesStack = scala.collection.mutable.Stack[Message]()
  
    
    
  def message(baseName : String)(cl : => Any) : Unit = {
    
    messagesStack.push(baseName)
    cl
    messagesStack.pop
    
    //baseName is cl
    
  }
  
  /**
   * Add Current Base message as Element with "Request" at the end
   */
  def request(cl : => Any) = {
    
    messagesStack.headOption match {
      case Some(message) => 
        
        message.baseName+"Request" is cl
        
      case None => 
    }
    
  } 
  
  /**
   * Add Current Base message as Element with "Response" at the end
   */
  def response(cl : => Any) = {
    
    messagesStack.headOption match {
      case Some(message) => 
        
        message.baseName+"Response" is cl
        
      case None => 
    }
    
  } 
  
  
}