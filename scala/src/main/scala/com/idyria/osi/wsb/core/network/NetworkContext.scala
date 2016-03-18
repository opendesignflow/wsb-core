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
package com.idyria.osi.wsb.core.network

import com.idyria.osi.tea.listeners.ListeningSupport

/**
 *
 * The NetworkContext Class is used by messages to refer back to the network layer they were send/from to
 *
 */
class NetworkContext extends ListeningSupport {

  /**
   * Attachments map to store any kind of extra objects in the network context
   */
  var attachments = Map[String, Any]()

  def apply(t: (String,Any) ) = {
    this.attachments = this.attachments + t
  }
  
  def apply[T <: Any](t: String) : Option[T] = {
    this.attachments.get(t) match {
      case Some(v) => Some(v.asInstanceOf[T])
      case None => None
    }
  }
  
  /**
   * The qualifier should uniquely identify the Context, so that connectors can initiate connections or send responses to the right client
   */
  var qualifier: String = null

  override def toString: String = qualifier match {
    case null => qualifier.toString
    case _    => qualifier
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
        case Some(m) => Some(m.group(1), m.group(2), m.group(3))
        case None    => None
      }

    }

  }

}