package com.idyria.osi.wsb.lib.soap

import com.idyria.osi.wsb.lib.client.WSBClientEngine
import com.idyria.osi.wsb.core.broker.tree.Intermediary
import com.idyria.osi.ooxoo.core.buffers.structural.ElementBuffer
import com.idyria.osi.wsb.core.message.soap.SOAPMessage
import com.idyria.osi.wsb.core.message.UpMessage
import com.idyria.osi.wsb.core.message.Message
import com.idyria.osi.wsb.core.network.NetworkContext
import com.idyria.osi.wsb.core.message.soap.SOAPIntermediary
import com.idyria.osi.wsb.lib.client.WSBClientEngine
import com.idyria.osi.wsb.core.message.soap.SOAPMessagesHandler
import com.idyria.osi.wsb.core.broker.tree.Intermediary
import com.idyria.osi.wsb.core.message.soap.SOAPMessage
import com.idyria.osi.wsb.lib.client.WSBClientEngine
import com.idyria.osi.wsb.core.message.soap.Fault
import com.idyria.osi.aib.core.bus.aib
import com.idyria.osi.wsb.core.WSBEngine
import com.idyria.osi.wsb.core.network.connectors.AbstractConnector

/**
 *
 * A Client Engine with default WSA intermediary as first broker child
 *
 */
class WSAClientEngine(e: WSBEngine = new WSBEngine) extends WSBClientEngine(e) {

  // Init/Start/Start
  //-----------------
  def start = {
    engine.lInit
    engine.lStart
  }
  
  def stop = {
    engine.lStop
  }

  //-- Connector
  //---------------------
  def apply(connector:AbstractConnector[_ <: NetworkContext]) = {
    this.engine.network.addConnector(connector)
  }
  
  //-- Add WSA
  //------------------
  var wsaIntermediary = new WSAIntermediary
  this.engine.broker <= wsaIntermediary
  this.engine.broker.brokeringTree.upStart = wsaIntermediary

  //-- Intermediary add directly through engine shortcuts to wsaIntermediary
  def <=(int: Intermediary) = wsaIntermediary <= int

  //-- Add  SOAPMessage Intermerdiary for responses
  //------------------------
  val responsesHandler = new SOAPMessagesHandler {

    //onDownMessage[Fau]

    // Errors
    //---------------
    on[Fault] {
      (message, f) => aib ! f
    }

  }
  this <= responsesHandler

  // Send 
  //-------------------

  /**
   * Send a Created SOAP Message with message as payload to the network
   *
   * The provided response closure is consumable and will be removed once the response has been received
   */
  def send[T <: ElementBuffer, RT <: ElementBuffer](payload: T, networkId: String)(respClosure: RT => Unit) = {

    // Get Type of response
    //-----------------
    var closureMethod = respClosure.getClass().getMethods().filter {
      m => m.getName() == "apply" && m.getReturnType() == Void.TYPE
    }.head

    var elementType = (closureMethod.getParameterTypes()(0).asInstanceOf[Class[RT]])

    // Register Response closure
    //-------------------
    responsesHandler.onMessageType(elementType, {
      (message, m: RT) =>

        try {
          respClosure(m)
        } finally {

          // Consume closure
          responsesHandler.removeHandler(respClosure)

        }

    })

    // Create Message
    //----------------
    var message = new SOAPMessage with UpMessage

    message.body.content += payload

    message.qualifier = Message.Qualifier(payload)

    // Set network context to string
    //--------------
    message.networkContext = NetworkContext(networkId)

    // Send
    //----------
    engine ! message

  }

}

object WSAClientEngine {

  var intern = new WSAClientEngine

  def apply(): WSAClientEngine = this.intern

}
