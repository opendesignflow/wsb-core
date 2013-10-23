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
import java.util.regex.Pattern
import com.idyria.osi.tea.logging.TLogSource

/**
 * @author rleys
 *
 */
trait Intermediary extends ElementBuffer with TLogSource {

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
  // -- The Closures are stored in a list of closures that all must return true for the message to be accepted
  //---------------
  
  /**
   * Per default always accept message
   */
  var acceptDownClosures: List[Message => Boolean] = List({ m=> true})
  
  /**
   * Define the Closure used to accept a messagein the down direction
   * 
   */
  def acceptDown(cl: Message => Boolean) = acceptDownClosures = acceptDownClosures :+ cl
  
  var downClosure: (Message => Unit) = null

  
   /**
   * Per default always accept message
   */
  var acceptUpClosures: List[Message => Boolean] = List({ m=> true})
  
  /**
   * Define the Closure used to accept a messagein the down direction
   * 
   */
  def acceptUp(cl: Message => Boolean) = acceptUpClosures = acceptUpClosures :+ cl
  
  var upClosure: (Message => Unit) = null
  
  /**
   * Up Actions can start somewhere else that the calling intermediary
   * 
   * For example, you can up messages from you top tree instance, and configure it to up from some sub intermediary
   * that add some default message processing, to avoid the user having to know where the start is located
   * 
   */
  var upStart : Intermediary = null
  
  // Up/Down runtime
  //---------------
  final def down(message: Message): Unit = {

    //println(s"[Down] Intermediary with filter: $filter with message: ${message.qualifier}")

    // Ignore message if pattern does not apply
    //--------------
    filter.findFirstMatchIn(message.qualifier) match {

      //-- Proceed locally and to descendants
      case Some(matchResult) if(acceptDownClosures.forall(_(message))) =>

        logInfo(s"${depthString("--")}  [Down] Accepted on ${this.name} with filter  $filter and message: ${message.qualifier}@${message.hashCode()} ")

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
         
        logInfo(s"${depthString("--")} [Down] Rejected $filter with message: ${message.qualifier}@${message.hashCode()} on ${this.name}")

      //println(s"---> Rejected")

    }

  }

  final def up(message: Message): Unit = {

     
     logInfo(s"${depthString("--")} [Up] Upstart is $upStart")
    // if the upStart is delocalized, and upping has not started, start at upStart
    if (this.upStart!=null && this.upStart!=this && message.upped == false) {
      
      logInfo(s"${depthString("--")} [Up] Upstart is delocalized")
      
      this.upStart.up(message)
      
    } 
    // Otherwise, everything should be normal
    else {
      
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
	        
	        println(s"${depthString("--")} [Up] Rejected Intermediary ${name} no Up closure")
	      
	        
	      case closure if( acceptUpClosures.forall(_(message))==false) =>
	        
	         println(s"${depthString("--")} [Up] Rejected Intermediary ${name} with filter: $filter with message: ${message.qualifier}")
	      
	        
	      case closure => 
	        
	        println(s"${depthString("--")} [Up] Accepted Intermediary ${name} with filter: $filter with message: ${message.qualifier}")
	      
	        closure(message)
	    }
	
	    // Pass to parent if possible
	    //---------------
	    if (this.parentIntermediary != null) {
	
	      this.parentIntermediary.up(message)
	
	    }
      
    }
    
    //println(s"[Up] Intermediary with filter: $filter with message: ${message.qualifier}")
	  
    

  }
  
  // Response
  //----------------------------

  /**
   * Sends up a message as response to another one
   */
  def response(responseMessage: Message, sourceMessage: Message): Unit = {

    throw new ResponseException(responseMessage)
   
  }
  def response(responseMessage: Message): Unit = {

    throw new ResponseException(responseMessage)
   
  }


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

  // Utilities
  //-----------------
  
  /**
   * Returns the depth of this intermediary
   */
  def depth : Int = {
    
    var current = this
    var count = 0
    var steps = while(current.parentIntermediary!=null) {
      current = current.parentIntermediary;
      count += 1
    } 
      
    count
    
  }
  
  /**
   * Returns a string formed by the separated repeated $depth times
   */
  def depthString(separator: String) : String = ( for ( i <- 0 to depth) yield separator ).mkString
  
}

class ResponseException(var responseMessage : Message) extends Exception {
  
}

object Intermediary {

  object Filter {
    
     /**
	   * Returns a filter regexp for the provided string, like this:
	   * 
	   * .*$str.*  with the str content beeing escaped
	   * 
	   */
	  def apply(str:String) : Regex = {
	    
	    s""".*${Pattern.quote(str)}.*""".r
	    
	  }
    
  }
  
}
