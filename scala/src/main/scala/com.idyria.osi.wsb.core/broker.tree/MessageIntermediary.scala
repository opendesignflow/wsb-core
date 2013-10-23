package com.idyria.osi.wsb.core.broker.tree

import com.idyria.osi.wsb.core.message.Message


/**
 * A Specialized intermediary that focuses on accepting a specific message type
 * 
 * The User can just provide some more convenient function to handle messages, and sub implementations 
 * can provide default behavior in case of errors for example
 * 
 * 
 */
trait MessageIntermediary[MT <: Message] extends Intermediary {
 
  
  // Message closure
  var messageDownClosure : ( MT => Unit) = { m => }
  var messageUpClosure : ( MT => Unit) = { m => }
  
  // Error closure
  var errorClosure : (Throwable => Unit) = { e => throw e}
  
  // User Interface
  //-----------------
  
  def messageType : Class[MT]
  
  def onDownMessage(cl: MT => Unit) = this.messageDownClosure = cl
  def onUpMessage(cl: MT => Unit) = this.messageUpClosure = cl
  
  def onError(cl: Throwable => Unit) = this.errorClosure=cl
  
  // Implementation
  //-----------------------
  
  //-- Only accept messages of provided type
  this.acceptDown{ m => messageType.isAssignableFrom(m.getClass()) }
  this.acceptUp{ m =>  messageType.isAssignableFrom(m.getClass())  }
  
  //-- Call on message closure
  this.downClosure = {
    
    m => 
    
      try {
    	  messageDownClosure(m.asInstanceOf[MT])
      } catch {
        case e : ResponseException => throw e
        case e : Throwable => this.errorClosure(e)
      }
      
  }
  
  //-- Call on message closure
  this.upClosure = {
    
    m => 
    
      try {
    	  messageUpClosure(m.asInstanceOf[MT])
      } catch {
        case e : ResponseException => throw e
        case e : Throwable => this.errorClosure(e)
      }
      
  }
  
}