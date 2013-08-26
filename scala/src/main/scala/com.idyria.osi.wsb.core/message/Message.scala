/**
 *
 */
package com.idyria.osi.wsb.core.message

import com.idyria.osi.wsb.core.network._

/**
 * @author rleys
 *
 */
class Message {

  /**
   * Qualifier used in tree processing to filter out subtrees
   */
  var qualifier = ""

  var networkContext : NetworkContext = null
  
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