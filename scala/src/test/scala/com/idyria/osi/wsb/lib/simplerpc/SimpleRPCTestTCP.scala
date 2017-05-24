package com.idyria.osi.wsb.lib.simplerpc

import com.idyria.osi.wsb.core.WSBEngine
import com.idyria.osi.wsb.core.broker.MessageBroker
import com.idyria.osi.tea.logging.TeaLogging
import com.idyria.osi.tea.logging.TLog
import com.idyria.osi.wsb.core.network.protocols.simple.SimpleMessageTCPConnector
import com.idyria.osi.wsb.core.network.connectors.AbstractConnector
import com.idyria.osi.wsb.core.network.connectors.tcp.TCPNetworkContext
import com.idyria.osi.wsb.core.network.connectors.tcp.TCPConnector

object SimpleRPCTestTCP extends App {
  
  //TLog.setLevel(classOf[TCPConnector], TLog.Level.FINE)
  //println(s"Test")
  
  // Define Test Class
  // "SimpleRPCMessage" Enables common type for RPC and correct generation of XML for network support
  //
  // The syntax "case class TestRequest( var msg : String) extends SimpleRPCMessage" is not possible right now, because the receiving part must instantiate the object with an empty constructor
  // It could be fixed, but I'd need a little bit of research
  //-------------------------
  class TestRequest extends SimpleRPCMessage {
    var msg : String = ""
    var value : Int = 0
  }
  
  //-------------------------------------
  // Server Side
  //-------------------------------------
  
  
  //--- We create a WSBEngine, which is the base object that starts/stops network connectors and message handlers
  var engine = new WSBEngine
  
  
  //--- Add A TCP Connector with a simple protocol to exchange message, and expecting "simplerpc" message types
  //---- The message type string is used when a message is received to find a factory that will transfor the raw data to an object
  //---- "simplerpc" maps to the object "SimpleRPCMessage" which is a message factory (See class definition)
  //--- Address is localhost, and port 8888
  var conn = new SimpleMessageTCPConnector
  conn.port = 8888
  conn.messageType = "simplerpc"
  engine.network.addConnector(conn)
  
  
  // Start the engine: It will start the TCP connection thread waiting for connections
  engine.lInit
  engine.lStart
  
  // The broker receives messages from the network side, and dispatches the messages to handlers
  // This way you can handler various kind of messages (simple RPC; HTTP or whatever you like)
  //
  // Here we add a SimpleRPCMessagesHandler to the Broker, which will accept messages of type "SimpleRPCMessage"
  ///--------------------
  engine.broker <= new SimpleRPCMessagesHandler {
    
    // The on[TestRequest] call means the closure { request => } will be called everytime a message with type TestRequest is passed to the handler 
    // If a SimpleRPCMessage of another type is seen, but has no match on[....] registered closure, it will be ignored
    on[TestRequest] {
      
      // Request is of type TestRequest
      request => 
        
        // Received something
        println(s"TestRequest Received: "+request.msg+" -> "+request.value)
        
        // No need to generate a response here because we are in loop-back mode
    }
  }
  
  
  
  
  //-------------------------------------
  // Client Side
  //-------------------------------------
  
  // Create a Request Object
  //-------------------------
  
  
  // Here on the client side, you could send it by TCP/IP to the server
  // For now, we just dispatch it to the broker directly as if in loop-back mode
  
  //engine ! req
  
  
  //---- Create Client Side Engine and start it
  var clientEngine = new WSBEngine
  
  clientEngine.lInit
  clientEngine.lStart
  
  //---- Create a request to send to the server part
  var req = new TestRequest
  req.msg = "Hello"
  req.value = 10
  
  // Debug?
  //println(s"Request: "+req.toXMLString)
  
  //---- MANDATORY: To set the destination of the message, a network context object must be set on the message object
  //---- The Context can be described by a String, like an URL, with the format: protocols://address:port
  //---- The "protocols" part here is complex because the API needs to know the full transmission stack to create the client connector automatically
  //---- Description of tcp+simple+simplerpc is:
  //-------   "tcp" means it should be a TCP connector
  //-------   "simple" means it should be the simple protocol to exchange message
  //-------   "simplerpc" means the message type is simplerpc (like on server side)
  //
  // When the message is send, a Connector implementation matching tcp+simple will be found (standard in the library), instantiated and connected to the server side
  req.networkContext = Some(new TCPNetworkContext("tcp+simple+simplerpc://localhost:8888"))
  
  // Now jut send the message (connector matching and instantiation happens here)
  clientEngine.network.send(req)
  
  // Just a wait to make sure the engine's Message Thread processes before exiting
  Thread.sleep(2000)
  engine.lStop
  clientEngine.lStop
  
  
  
  
  
  
  
}