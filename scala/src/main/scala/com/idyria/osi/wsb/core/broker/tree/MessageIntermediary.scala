/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2014 OSI / Computer Architecture Group @ Uni. Heidelberg
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/gpl-3.0.html>.
 * #L%
 */
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
  this.acceptDown{ m => 
    //println(s"MI accepting? -> "+m.getClass.getSuperclass+"->"+messageType.isAssignableFrom(m.getClass()))
    messageType.isAssignableFrom(m.getClass()) }
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