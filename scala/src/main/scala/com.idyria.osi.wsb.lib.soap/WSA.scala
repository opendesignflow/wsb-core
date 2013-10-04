package com.idyria.osi.wsb.lib.soap


import com.idyria.osi.wsb.core.message.soap.SOAPMessage
import com.idyria.osi.ooxoo.core.buffers.structural.xelement
import com.idyria.osi.ooxoo.core.buffers.structural.ElementBuffer
import com.idyria.osi.wsb.core.broker.tree.Intermediary
import com.idyria.osi.ooxoo.core.buffers.structural.AnyXList
import com.idyria.osi.ooxoo.core.buffers.datatypes.XSDStringBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.DataUnit
import com.idyria.osi.wsb.core.message.soap.SOAPIntermediary


/**
 * This intermediary sets or extracts qualifier of a message using Webservice addressing structures
 */
class WSAIntermediary extends SOAPIntermediary {
  
  this.name = "WSA"
  
  // Register XML Elements with AnyContent for SOAP
  //---------------
  AnyXList(classOf[Action])
  
  
  // Message Handler
  //-------------------
  this.onDownMessage {
    
    soapMessage => 
   
      println("[WSA] incoming: "+soapMessage.toXMLString)
      
      //-- Look for Action in header
      //---------
      soapMessage.header.content.foreach {
        case action : Action => 
          	
          soapMessage.qualifier = action.toString()
        
          println("WSA action: "+action.toString())
        
        case h => 
          
          println("WSA header: "+h)
      }
      
  }
  
  this.onUpMessage {
    soap => 
      
      // Add/Update Action in Header from qualifier
      //---------------------------------
      soap.header.content.find {
        
        // Found action, update
        case action : Action => 
          	
         	action.data = soap.qualifier
         	true
        
        case _ =>  false 
          
      } match {
        
        // No action, add
        case None => 
          
           var action = new Action
           action.data = soap.qualifier
           soap.header.content+=action
          
        case _ =>
      }
      
      println("[WSA] post wsa: "+soap.toXMLString)
      
  }
 
  
}
 

@xelement(name="Action",ns="http://schemas.xmlsoap.org/ws/2004/08/addressing")
class Action extends XSDStringBuffer {
  
 /* override def streamIn(du: DataUnit) = {
    println("Got DataUnit in Action")
  }*/
  
  
  
}
