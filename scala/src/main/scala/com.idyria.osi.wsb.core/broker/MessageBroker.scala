package com.idyria.osi.wsb.core.broker

import com.idyria.osi.wsb.core.message.Message
import com.idyria.osi.wsb.core.broker.tree.BrokeringTree
import com.idyria.osi.wsb.core._

class MessageBroker ( var engine : WSBEngine ) extends Lifecycle {

  /**
   * The default brokering tree used. can be easily improved by API
   */
  var brokeringTree = new BrokeringTree()


  // Engine Connection
  //-------------

  //-- Listen for Messages to be handled

  /**
    Method to send an event to the local Engine internal Bus
  */
  def ! ( msg :  AnyRef) = this.engine ! msg


  // Message Handling
  //-------------------------

  def handleMessage(message:Message) = {

  }

  def down(message:Message) = {

  }


  // Lifecycle
  //------------------------

  /**
    Init Message Broker

  */
  def lInit = {



  }

  /**
   * Start Message Broker
   */
  def lStart = {

    // Nothing to do

  }

  def lStop = {

    // Nothing to do

  }




}
object MessageBroker {



}
