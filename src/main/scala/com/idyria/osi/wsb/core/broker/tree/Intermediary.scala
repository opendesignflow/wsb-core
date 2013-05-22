/**
 *
 */
package com.idyria.osi.wsb.core.broker.tree

import scala.util.matching.Regex
import com.idyria.osi.ooxoo3.core.buffers.structural.XList
import com.idyria.osi.ooxoo3.core.buffers.structural.ElementBuffer
import com.idyria.osi.ooxoo3.core.buffers.structural.xattribute
import com.idyria.osi.ooxoo3.core.buffers.structural.xelement

/**
 * @author rleys
 *
 */
class Intermediary extends ElementBuffer {

  /**
   * Filter is used by Broker to determine if a message should go through this intermediary.
   */
  @xattribute
  var filter : Regex = null
  
  @xelement()
  var intermediaries : XList[Intermediary] = new XList[Intermediary] {
    	def createBuffer() : Intermediary = {
    	  new Intermediary
    	}
  }
  
}