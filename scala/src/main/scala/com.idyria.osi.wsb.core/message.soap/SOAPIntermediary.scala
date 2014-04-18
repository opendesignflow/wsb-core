package com.idyria.osi.wsb.core.message.soap

import com.idyria.osi.wsb.core.broker.tree.MessageIntermediary
import com.idyria.osi.ooxoo.core.buffers.structural.ElementBuffer
import com.idyria.osi.wsb.core.network.NetworkContext




/**
 * Specialised intemerdiary for SOAPHandling
 * 
 * Returns SOAP Faults on errors
 */
trait SOAPIntermediary extends MessageIntermediary[SOAPMessage] {

 
  // Message Type
  //--------------------
  def messageType = classOf[SOAPMessage]
  
  // Error Handling
  //---------------------
  onError {
    
    e => 
    
      
      e.printStackTrace()
      
    // Create Fault
    //------------------
    var f = new Fault
    f.reason.text =  s"${e.getClass().getCanonicalName()}:${e.getLocalizedMessage()}"

    
    // Add To new SOAPMessage
    //-------------
    var resp = new SOAPMessage
    resp.body.content+=f
    resp.qualifier = com.idyria.osi.wsb.core.message.Message.Qualifier(f)
      
    // Return
    //------------------
    super.response(resp)
    
  }
  
  // Response
  //----------------
  def response(payload: ElementBuffer) = {
    
    // Add To new SOAPMessage
    //-------------
    var resp = new SOAPMessage
    resp.body.content+=payload
    
    // Qualifier is extracted from payload
    //--------------
    resp.qualifier = com.idyria.osi.wsb.core.message.Message.Qualifier(payload)

    
    // Return
    //------------------
    super.response(resp)
    
  }
  
  // Up with Qualifier
  //----------------
  
  /**
   * Up default SOAPMessage with qualifier taken from com.idyria.osi.wsb.core.message.Message.Qualifier default extractor
   */
  def up(payload:ElementBuffer,nc: NetworkContext) : Unit =  {
    up(com.idyria.osi.wsb.core.message.Message.Qualifier(payload),payload,nc)
  }
  
  /**
   * Up default SOAPMessage with qualifier
   */
  def up(qualifier: String,payload:ElementBuffer,nc: NetworkContext) : Unit = {
    
    // Create new SOAPMessage
    //-------------
    var resp = new SOAPMessage
    resp.networkContext = nc
    resp.body.content+=payload
    
    resp.qualifier = qualifier
    
    //println("[Upping] "+resp.toXMLString)
    
    // Return
    //------------------
    super.up(resp)
    
  }
  
  
}