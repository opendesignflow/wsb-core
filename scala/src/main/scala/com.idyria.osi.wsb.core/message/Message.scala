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

  var networkContext : NetworkContext = null

    /**
        If for example, this message is a response to another one, this is the request message
    */
    var relatedMessage : Message = null

  /**
    To be implemented by messages for connectors to retrieve bytes for this message
  */
  def toBytes : ByteBuffer

}

trait MessageFactory {


	def apply(data: Any) : Message 

}

object Message {

	var messageFactories = Map[String,MessageFactory]()

	/**
		Returns a Message Instance matching the given message type
	*/
	def apply(messageType: String) : Option[MessageFactory] = messageFactories.get(messageType)

	def apply(messageType: String,factory: MessageFactory) = {

		messageFactories = messageFactories + (messageType -> factory)

	}

}

