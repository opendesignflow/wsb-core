/**
 *
 */
package com.idyria.osi.wsb.core

/**
 * @author rleys
 *
 */
trait Lifecycle {

  private var state = "none"

  // Lifecycle Implementation
  //-----------------------------------

  protected def lInit

  protected def lStart

  protected def lSuspend = {

  }

  protected def lResume = {

  }

  protected def lStop

  protected def lClean = {

  }

  // Lifecycle Management
  //--------------------------

  /**
   * Progess to targetState
   */
  private def cycleTo(targetState: String): Unit = {

    (this.state, targetState) match {

      case ("none", "init") ⇒

        this.lInit
        this.state = "init"

      // Start
      case (actual, "start") if (actual!="start") ⇒

        cycleTo("init")
        this.lStart
        this.state = "start"

      // Stop
      case ("start", "stop") ⇒

        this.lStop
        this.state = "stop"

      case (_, "stop") ⇒ throw new RuntimeException("Can only cycle to stop from start")
      
      
      // Ignore
      case _           ⇒

    }

  }

  def cycleToInit = cycleTo("init")

  def cycleToStart = cycleTo("start")

  def cycleToStop = cycleTo("stop")

}
