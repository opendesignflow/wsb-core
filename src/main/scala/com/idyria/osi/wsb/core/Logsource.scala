/**
 *
 */
package com.idyria.osi.wsb.core

/**
 * @author rleys
 *
 */
trait Logsource {

  var logger = getClass().getCanonicalName()
  
  def logInfo( message : String) = println(s"$logger $message")
  
}