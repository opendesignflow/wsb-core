package com.idyria.osi.wsb.lib.simplerpc

import com.idyria.osi.wsb.core.WSBEngine
import com.idyria.osi.wsb.core.broker.MessageBroker
import com.idyria.osi.tea.logging.TeaLogging
import com.idyria.osi.tea.logging.TLog

object SimpleRPCTest extends App {
  
  
  println(s"Test")
  
  // Define Test Class
  // "SimpleRPCMessage" Enables common type for RPC and correct generation of XML for network support
  //-------------------------
  case class TestRequest( var msg : String) extends SimpleRPCMessage 
  
  
  //TLog.setLevel(classOf[SimpleRPCMessagesHandler], TLog.Level.FINE)
  
  // Create Handling Engine
  // We create a WSBEngine, without Network (TCP) connection for the moment
  //----------------------
  var engine = new WSBEngine
 
  // Start the engine (will also start the TCP Threads if any registered)
  engine.lInit
  engine.lStart
  
  // The broker receives messages from the network side when present, and dispatches the messages to handlers
  // This way you can handler various kind of messages (simple RPC; HTTP or whatever you like)
  //
  // Here we add a SimpleRPCMessagesHandler to the Broker
  ///--------------------
  engine.broker <= new SimpleRPCMessagesHandler {
    
    // The on[TestRequest] call means the closure { request => } will be called everytime a message with type TestRequest is passed to the handler 
    on[TestRequest] {
      
      // Request is of type TestRequest
      request => 
        
        // Received something
        println(s"TestRequest Received: "+request.msg.toString)
        
        // No need to generate a response here because we are in loop-back mode
    }
  }
  
  
  
  
  
  
  // Create a Request Object
  //-------------------------
  var req = TestRequest("Hello")
  println(s"Request: "+req.toXMLString)
  
  // Here on the client side, you could send it by TCP/IP to the server
  // For now, we just dispatch it to the broker directly as if in loop-back mode
  
  engine ! req
  
  // Just a wait to make sure the engine's Message Thread processes before exiting
  Thread.sleep(1000)
  engine.lStop
  
  
  
  
  
  
  
}