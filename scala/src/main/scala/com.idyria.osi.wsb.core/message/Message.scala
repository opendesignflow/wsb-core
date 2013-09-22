/**
 *
 */
package com.idyria.osi.wsb.core.message

import com.idyria.osi.wsb.core.network._

import java.nio._
/**
 * @author rleys
 *
 */
abstract class Message {

  /**
   * Qualifier used in tree processing to filter out subtrees
   */
  var qualifier = ""

  var networkContext: NetworkContext = null

  /**
   * If for example, this message is a response to another one, this is the request message
   */
  var relatedMessage: Message = null

  /**
   * Set to true if this message or a response message has been upped (answered so to say)
   */
  var upped = false
  
  
  /**
   * To be implemented by messages for connectors to retrieve bytes for this message
   */
  def toBytes: ByteBuffer
  
  // Error Handling
  //-----------------
  
  /**
   * if an intermediary fails by throwing an exception, this doesn't mean the whole tree processing fails, because handling might differ based on applications
   * However, errors are stored here so that some special intermediary can log errors or do some other stuff
   */
  var errors =  List[Throwable]()
  
  /**
   * Execute closure on all errors, and remove errors from list
   * @return the Gathered Result list
   */
  def consumeErrors(cl: Throwable => Any) : List[Any] = {
    
    var res = List[Any]()
    this.errors = this.errors.filter {
      case e : Throwable => res = cl(e) :: res ; false
    }
    
    res
    
  }
  
  
  
  // Apply functions
  //---------
  
  /**
   * Record an error into this message
   */
  def apply(e: Throwable) = {
    
    this.errors = e :: this.errors
    
  }

}

trait MessageFactory {

  def apply(data: Any): Message

}

object Message {

  var messageFactories = Map[String, MessageFactory]()

  /**
   * Returns a Message Instance matching the given message type
   */
  def apply(messageType: String): Option[MessageFactory] = messageFactories.get(messageType)

  def apply(messageType: String, factory: MessageFactory) = {

    messageFactories = messageFactories + (messageType -> factory)

  }

}

