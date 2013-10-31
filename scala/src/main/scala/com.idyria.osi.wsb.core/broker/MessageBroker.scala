package com.idyria.osi.wsb.core.broker

import com.idyria.osi.wsb.core._
import com.idyria.osi.wsb.core.broker.tree._
import com.idyria.osi.wsb.core.message.Message
import com.idyria.osi.wsb.core.message.UpMessage
import com.idyria.osi.tea.logging.TLogSource

  
class MessageBroker ( var engine : WSBEngine ) extends Lifecycle with TLogSource {

   
 
  // Brokering Tree
  //---------------------

  /**
   * The default brokering tree used. can be easily improved by API
   */
  var brokeringTree = new Intermediary {


    /**
      Send message through network
    */
    upClosure = {
      message => 

        engine.network.send(message)

    }

  }

   
  val messageReceiveClosure = {

      msg : Message => 

        msg match {
          case m : UpMessage => 
            
            logFine("received Message to up") 
            
            brokeringTree.up(msg)
            
          case _ => 
            
            logFine("Broker Got Down Message: "+msg.getClass) 
            logFine("-> Qualifier "+msg.qualifier) 
            brokeringTree.down(msg) 
        }
        
          

          // broker
          

  }

  /**
    Add an intermediary to the root  Intermediary
  */
  def <=(intermediary: Intermediary) = brokeringTree.<=(intermediary)


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

    // Register Message Closure to aib
    engine.localBus.registerClosure(messageReceiveClosure)


 
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
