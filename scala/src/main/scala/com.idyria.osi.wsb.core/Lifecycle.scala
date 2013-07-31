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

  protected def lSuspend =  {

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
  private def cycleTo(targetState : String) = {

    if (targetState=="init") {
      this.lInit
      this.state="init"
    }


    if (targetState=="start") {

      if (state=="none") {
        this.lInit
      }
      this.lStart
      this.state = "start"
    }

    if (targetState=="stop") {

      // If not in state start, this is an error
      if (state!="start")
        throw new RuntimeException("Can only cycle to stop from start")
      this.lStop
      this.state = "stop"
    }

  }

  def cycleToInit = cycleTo("init")

  def cycleToStart = cycleTo("start")

  def cycleToStop = cycleTo("stop")



}
