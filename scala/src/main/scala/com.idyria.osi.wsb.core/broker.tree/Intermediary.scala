/**
 *
 */
package com.idyria.osi.wsb.core.broker.tree

import com.idyria.osi.wsb.core.message.Message

import scala.util.matching.Regex
import com.idyria.osi.ooxoo.core.buffers.structural.XList
import com.idyria.osi.ooxoo.core.buffers.structural.ElementBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.xattribute
import com.idyria.osi.ooxoo.core.buffers.structural.xelement
import com.idyria.osi.ooxoo.core.buffers.datatypes._

/**
 * @author rleys
 *
 */
trait Intermediary extends ElementBuffer {

  /**
   * A Name for user/api to formally identify the intermediary
   */
  @xattribute
  var name: XSDStringBuffer = ""

  /**
   * Filter is used by Broker to determine if a message should go through this intermediary.
   */
  @xattribute
  var filter: Regex = """.*""".r

  @xelement
  var intermediaries: XList[Intermediary] = XList[Intermediary] { new Intermediary {} }

  /**
   * A parent Intermediary if defined, mainly for up operation
   */
  var parentIntermediary: Intermediary = null
  
  // Up/ Down closures for user processing
  //---------------
  
  /**
   * Per default always accept message
   */
  var acceptDownClosure: (Message => Boolean) = { m=> true}
  
  /**
   * Define the Closure used to accept a messagein the down direction
   * 
   */
  def acceptDown(cl: Message => Boolean) = acceptDownClosure = cl
  
  var downClosure: (Message => Unit) = null

  
   /**
   * Per default always accept message
   */
  var acceptUpClosure: (Message => Boolean) = { m=> true}
  
  /**
   * Define the Closure used to accept a messagein the down direction
   * 
   */
  def acceptUp(cl: Message => Boolean) = acceptUpClosure = cl
  
  var upClosure: (Message => Unit) = null

  // Up/Down runtime
  //---------------
  final def down(message: Message): Unit = {

    //println(s"[Down] Intermediary with filter: $filter with message: ${message.qualifier}")

    // Ignore message if pattern does not apply
    //--------------
    filter.findFirstMatchIn(message.qualifier) match {

      //-- Proceed locally and to descendants
      case Some(matchResult) if(acceptDownClosure(message)==true) =>

        println(s"---> Accepted $filter with message: ${message.qualifier}@${message.hashCode()} on ${this.name}")

        // Local closure
        //-------------
        downClosure match {
          case null => 
          case closure => 
            
            try {
    
            	closure(message)
          
            } catch {
              case e : ResponseException => 
                
                // Copy context
			    e.responseMessage.networkContext = message.networkContext
			
			    // Set related message
			    e.responseMessage.relatedMessage = message
			
			    // Up :)
			    up(e.responseMessage)
			    
			    //throw e
              
			  // In case of error, record to message
              case e : Throwable => message(e)
                	
            } finally {
            	
            	
            }
        }
        
        // Pass to children if closure did not throw anything out
        try {
	    	this.intermediaries.foreach{
	    	  i => 
	    	    i.down(message)
	    	}
    	} catch {
	      case e : Throwable => throw e
	    }
    	
      //-- Ignore
      case _ =>

      //println(s"---> Rejected")

    }

  }

  final def up(message: Message): Unit = {

    //println(s"[Up] Intermediary with filter: $filter with message: ${message.qualifier}")
	  
    // Set Upped on message and related if any
    //--------
    message.upped = true
    if (message.relatedMessage!=null) {
      message.relatedMessage.upped = true
    }
    
    // Up Closure
    //-------------
    upClosure match {
      case null => 
      case closure if(acceptUpClosure(message)==false) =>
      case closure => closure(message)
    }

    // Pass to parent if possible
    //---------------
    if (this.parentIntermediary != null) {

      this.parentIntermediary.up(message)

    }

  }

  /**
   * Sends up a message as response to another one
   */
  def response(responseMessage: Message, sourceMessage: Message): Unit = {

    throw new ResponseException(responseMessage)
   
  }
  def response(responseMessage: Message): Unit = {

    throw new ResponseException(responseMessage)
   
  }


  def intermediaryTest = println("Hi!")

  // Language
  //-------------------

  /**
   * Add an intermediary to this current intermediary
   * 
   * @return The added intermediary for nicer api usage
   */
  def <=(intermediary: Intermediary) : Intermediary = {

    intermediaries += intermediary
    intermediary.parentIntermediary = this

    intermediary
  }

}

class ResponseException(var responseMessage : Message) extends Exception {
  
}

object Intermediary {

}
