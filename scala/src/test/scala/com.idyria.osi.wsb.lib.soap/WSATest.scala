package com.idyria.osi.wsb.lib.soap

import org.scalatest.FunSuite
import com.idyria.osi.wsb.core.message.soap.SOAPMessage
import java.nio.ByteBuffer
import com.idyria.osi.ooxoo.core.buffers.structural.AnyXList

/**
 *
 */
class WSATest extends FunSuite {

  test("Action Writeout") {
    
    var soapMessage = new SOAPMessage
    
     var action = new Action
     action.data = "Test"
     soapMessage.header.content += (action)
    
     var str = soapMessage.toXMLString
     
     println("Write Out result: "+str)
     
     expectResult(true)(str.matches(""".*Action.*>.*Test.*"""))
    
  }
  
  test("Action Parse") {

    var soapBase = <Envelope xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns="http://www.w3.org/2003/05/soap-envelope"><Header><wsa:Action>Test</wsa:Action></Header><Body></Body></Envelope>

    // Register Action to Any Content
    AnyXList(classOf[Action])

    
    AnyXList.modelsMap.foreach {
      case ( eltDefinition,bClass) => println(s"Available any: $eltDefinition -> $bClass")
    }
    
    // Parse
    //-----------
    println("Parsing: "+soapBase.toString)
    var soap = SOAPMessage(ByteBuffer.wrap(soapBase.toString.getBytes))

    soap.header.content.foreach {
      b => println(s"In header buffer: $b")
    }
    
    var actionHeader = soap.header.content.find(_.getClass == classOf[Action])
    expectResult(true)(actionHeader.isDefined)

    expectResult("Test")(actionHeader.get.toString)

  }

}