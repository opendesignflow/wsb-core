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
import scala.util.matching.Regex
import com.idyria.osi.ooxoo.core.buffers.structural.XList
import com.idyria.osi.ooxoo.core.buffers.structural.ElementBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.xattribute
import com.idyria.osi.ooxoo.core.buffers.structural.xelement
import com.idyria.osi.ooxoo.core.buffers.datatypes._
import java.util.regex.Pattern
import com.idyria.osi.tea.logging.TLogSource
import com.idyria.osi.tea.listeners.ListeningSupport
import scala.reflect.ClassTag

/**
 * @author rleys
 *
 */
trait Intermediary[MT <: Message] extends ElementBuffer with TLogSource with ListeningSupport {

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
  var intermediaries: XList[Intermediary[MT]] = XList[Intermediary[MT]] { new Intermediary[MT] {} }

  /**
   * A parent Intermediary[MT] if defined, mainly for up operation
   */
  var parentIntermediary: Intermediary[MT] = null

  // Up/ Down closures for user processing
  // -- The Closures are stored in a list of closures that all must return true for the message to be accepted
  //---------------

  /**
   * Per default always accept message
   */
  var acceptDownClosures: List[MT => Boolean] = List({ m => true })

  /**
   * Define the Closure used to accept a messagein the down direction
   *
   */
  def acceptDown(cl: MT => Boolean) = acceptDownClosures = acceptDownClosures :+ cl

  var downClosure: (MT => Unit) = null

  /**
   * Per default always accept message
   */
  var acceptUpClosures: List[Message => Boolean] = List({ m => true })

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
  var upStart: Intermediary[MT] = null

  // Up/Down runtime
  //---------------

  /**
   * Run message on the subtree of current intermediary
   */
  final def downTree(message: MT): Unit = {
    // Pass to children if closure did not throw anything out
    try {
      this.intermediaries.foreach {
        i =>
          i.down(message)
      }
    } catch {
      case e: Throwable => throw e
    }

  }
  final def down(message: MT): Unit = {

    //println(s"[Down] Intermediary[MT] with filter: $filter with message: ${message.qualifier}")

    // Ignore message if pattern does not apply
    //--------------
    filter.findFirstMatchIn(message.qualifier) match {

      //-- Proceed locally and to descendants
      case Some(matchResult) if (acceptDownClosures.forall(_(message))) =>

        logInfo[this.type](s"${depthString("--")}  [Down] Accepted on ${this.name} with filter  $filter and message: ${message.qualifier}@${message.hashCode()} ")

        // Local closure
        //-------------
        downClosure match {
          case null =>
          case closure =>

            try {

              closure(message)

            } catch {
              case e: ResponseException =>

                // Copy context
                e.responseMessage.networkContext = message.networkContext

                // Set related message
                e.responseMessage.relatedMessage = message

                // Up :)
                up(e.responseMessage)

              //throw e

              // In case of error, record to message
              case e: Throwable =>
                //e.printStackTrace()
                message(e)

            } finally {

            }
        }

        // Pass to children if closure did not throw anything out
        downTree(message)

      //-- Ignore
      case _ =>

        logInfo[this.type](s"${depthString("--")} [Down] Rejected $filter with message: ${message.qualifier}@${message.hashCode()} on ${this.name}")

      //println(s"---> Rejected")

    }

  }

  final def up(message: Message): Unit = {

    logInfo(s"${depthString("--")} [Up] Upstart is $upStart")
    // if the upStart is delocalized, and upping has not started, start at upStart
    if (this.upStart != null && this.upStart != this && message.upped == false) {

      logInfo[this.type](s"${depthString("--")} [Up] Upstart is delocalized")

      this.upStart.up(message)

    } // Otherwise, everything should be normal
    else {

      // Set Upped on message and related if any
      //--------
      message.upped = true
      if (message.relatedMessage != null) {
        message.relatedMessage.upped = true
      }

      // Up Closure
      //-------------
      upClosure match {
        case null =>

          logInfo[this.type](s"${depthString("--")} [Up] Rejected Intermediary[MT] ${name} no Up closure")

        case closure if (acceptUpClosures.forall(_(message)) == false) =>

          logInfo[this.type](s"${depthString("--")} [Up] Rejected Intermediary[MT] ${name} with filter: $filter with message: ${message.qualifier}")

        case closure =>

          logInfo[this.type](s"${depthString("--")} [Up] Accepted Intermediary[MT] ${name} with filter: $filter with message: ${message.qualifier}")

          closure(message)
      }

      // Pass to parent if possible
      //---------------
      if (this.parentIntermediary != null) {

        this.parentIntermediary.up(message)

      }

    }

    //println(s"[Up] Intermediary[MT] with filter: $filter with message: ${message.qualifier}")

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
  def <=(intermediary: Intermediary[MT]): Intermediary[MT] = {

    intermediaries += intermediary
    intermediary.parentIntermediary = this
    intermediary.@->("parent.new")
    intermediary
  }

  /**
   * Add an intermediary to this current intermediary
   *
   * @return The added intermediary for nicer api usage
   */
  def prepend(intermediary: Intermediary[MT]): Intermediary[MT] = {

    var newIn = XList[Intermediary[MT]] { new Intermediary[MT] {} }
    newIn += intermediary
    intermediary.parentIntermediary = this
    intermediaries.foreach(newIn += _)
    intermediaries = newIn

    intermediary
  }

  /**
   * Remove an intermediary to this current intermediary
   *
   * @return The removed intermediary for nicer api usage
   */
  def -=(intermediary: Intermediary[MT]): Intermediary[MT] = {

    var newIn = XList[Intermediary[MT]] { new Intermediary[MT] {} }
    intermediaries.filterNot { _ == intermediary }.foreach(newIn += _)
    intermediaries = newIn

    intermediary.parentIntermediary = null

    intermediary
  }

  // Utilities
  //-----------------

  /**
   * Returns the depth of this intermediary
   */
  def depth: Int = {

    var current = this
    var count = 0
    var steps = while (current.parentIntermediary != null) {
      current = current.parentIntermediary;

      count += 1
    }

    count

  }

  /**
   * Returns a string formed by the separated repeated $depth times
   */
  def depthString(separator: String): String = (for (i <- 0 to depth) yield separator).mkString

  /**
   * Search first parent matching the provided function
   */
  def findParent(cl: Intermediary[MT] => Boolean): Option[Intermediary[MT]] = {

    var current = this.parentIntermediary
    var res: Option[Intermediary[MT]] = None
    while (current != null && res == None) {

      //-- Try to match
      cl(current) match {
        case true => res = Some(current)
        case false =>
      }

      //-- Update current
      current = current.parentIntermediary;

    }

    res

  }

  def findParentOfType[T <: Intermediary[MT]](implicit tag: ClassTag[T]): Option[T] = {

    this.findParent { p => 
      
     // println(s"Parent type search testing: ${p.getClass.getCanonicalName} against ${}")
      tag.runtimeClass.isAssignableFrom(p.getClass) }.asInstanceOf[Option[T]]

  }

}

class ResponseException(var responseMessage: Message) extends Exception {

}

object Intermediary {

  object Filter {

    /**
     * Returns a filter regexp for the provided string, like this:
     *
     * .*$str.*  with the str content beeing escaped
     *
     */
    def apply(str: String): Regex = {

      s""".*${Pattern.quote(str)}.*""".r

    }

  }

}