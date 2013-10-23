package com.idyria.osi.wsb.core.network

/**
 *
 * The NetworkContext Class is used by messages to refer back to the network layer they were send/from to
 *
 */
class NetworkContext {

  /**
   * Attachments map to store any kind of extra objects in the network context
   */
  var attachments = Map[String, Any]()

  /**
   * The qualifier should uniquely identify the Context, so that connectors can initiate connections or send responses to the right client
   */
  var qualifier: String = null

  override def toString: String = qualifier match {
    case null ⇒ qualifier.toString
    case _    ⇒ qualifier
  }

}

object NetworkContext {

  /**
   * Returns a new instance of network context with provided qualifier
   */
  def apply(contextQualifier:String) : NetworkContext = {
    var nc = new NetworkContext
    nc.qualifier = contextQualifier
    nc
  }
  
  /*obbject NetworkString(var protocolStack : String, var messageType : String, var connectionString: String) {
    
    
    
  }*/

  object NetworkString {

    /**
     *
     * From context string:
     *
     * - take part before "://"
     * - from "+" separated stack, the last one is the message type, so ignore it
     *
     */
    def unapply(str: String): Option[(String, String, String)] = {

      """(?:(.*)\+)?(.+)://(.*)""".r.findFirstMatchIn(str) match {
        case Some(m) ⇒ Some(m.group(1), m.group(2), m.group(3))
        case None    ⇒ None
      }

    }

  }

}
