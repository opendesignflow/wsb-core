/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2017 OpenDesignFlow.org
								Richard Leys (leys dot richard at gmail):
								2008-2014 University of Heidelberg (Computer Architecture group)
								2008-2014 Extoll GmbH (extoll.de)
								2014-2017 University of Karlsruhe (KIT) - ASIC and Detector Lab
 * %%
 * This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * #L%
 */
package com.idyria.osi.wsb.core.message.soap

import scala.collection.JavaConversions._
import scala.language.reflectiveCalls
import java.util.concurrent._
import java.net._
import org.scalatest._
import java.net.URLClassLoader
import java.net.URL
import java.io._
import java.nio._
import java.nio.charset._
import java.nio.channels._
import com.idyria.osi.ooxoo.core.buffers.structural._
import com.idyria.osi.ooxoo.core.buffers.structural.io.sax._
import com.idyria.osi.wsb.lib.soap.Action
import com.idyria.osi.ooxoo.core.buffers.datatypes.XSDStringBuffer
import com.idyria.osi.ooxoo.lib.json.JsonIO

@xelement(name = "TestBody")
class TestBody extends ElementBuffer {

  @xelement
  var testBodySub: TestBodySub = null

}

@xelement(name = "TestBodySub")
class TestBodySub extends XSDStringBuffer {

}

class SOAPMessageTest extends FunSuite with GivenWhenThen {

  @xelement(name = "TestHeader")
  class TestHeader extends ElementBuffer {
  }

  @xelement(name = "TestHeader2")
  class TestHeader2 extends ElementBuffer {
  }

  @xelement(name = "TestHeaderNS1", ns = "ws-core:test")
  class TestHeaderNS1 extends ElementBuffer {
  }

  test("Create and serialize an empty messsage") {

    // Create and print
    //---------------------

    var message = SOAPMessage()

    println(message.toXMLString)
    println(message.toXMLString)

  }

  var messageWithHeaders: String = null
  test("Create and serialize a messsage with some Generic ElementBuffers inside") {

    // Create and print
    //---------------------
    var message = SOAPMessage()

    // Add a dummy test header
    //-------------------
    var testHeader = new TestHeader
    message.header.content += testHeader

    var testHeader2 = new TestHeader2
    message.header.content += testHeader2

    assertResult(2)(message.header.content.size)

    messageWithHeaders = message.toXMLString
    println(messageWithHeaders)

  }

  test("Create and serialize a messsage with some Generic ElementBuffers inside and namespace") {

    // Create and print
    //---------------------
    var message = SOAPMessage()

    // Add a dummy test header
    //-------------------
    var testHeader = new TestHeaderNS1
    message.header.content += testHeader

    var testHeader2 = new TestHeaderNS1
    message.header.content += testHeader2

    assertResult(2)(message.header.content.size)

    var messageWithHeaders = message.toXMLString
    println(messageWithHeaders)

  }

  test("Stream in a simple message with custom header") {

    // Create and print
    //---------------------
    var message = SOAPMessage()
    message - StAXIOBuffer(messageWithHeaders)
    message.lastBuffer.streamIn

    // Check Header elements have been parsed
    //--------------------------
    assertResult(2)(message.header.content.size)

    // Check names
    //------------------
    assertResult("TestHeader")(message.header.content.head.asInstanceOf[AnyElementBuffer].name)
    assertResult("TestHeader2")(message.header.content.last.asInstanceOf[AnyElementBuffer].name)

  }

  var messageWithHeadersComplexer = """
    <env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope">
        <env:Header>
            <TestHeader name="hihi"></TestHeader>
            <ns1:TestHeader2 xmlns:ns1="tt"></ns1:TestHeader2>
            <TestHeader2></TestHeader2>
        </env:Header>
        <env:Body>
            <TestHeader></TestHeader>
            <TestHeader2></TestHeader2>
            <TestHeader></TestHeader>
            <TestHeader2></TestHeader2>
        </env:Body>
    </env:Envelope>

    """
  test("Stream in a simple message with custom header, body, and some attributes") {

    // Create and print
    //---------------------
    var message = SOAPMessage()
    message - StAXIOBuffer(messageWithHeadersComplexer)
    message.lastBuffer.streamIn

    // Check  elements have been parsed
    //--------------------------
    assertResult(3)(message.header.content.size)
    assertResult(4)(message.body.content.size)

    // Check names
    //------------------
    assertResult("TestHeader")(message.header.content.head.asInstanceOf[AnyElementBuffer].name)
    assertResult("TestHeader2")(message.header.content.last.asInstanceOf[AnyElementBuffer].name)

    assertResult("TestHeader")(message.body.content.head.asInstanceOf[AnyElementBuffer].name)
    assertResult("TestHeader2")(message.body.content.last.asInstanceOf[AnyElementBuffer].name)

    // Check Attributes
    //----------------------
    var firstHeader = message.header.content.head.asInstanceOf[AnyElementBuffer]
    assertResult(1)(firstHeader.content.size)

    var firstHeaderAttribute = firstHeader.content.head
    assertResult(true)(firstHeaderAttribute.isInstanceOf[AnyAttributeBuffer])
    assertResult("name")(firstHeaderAttribute.asInstanceOf[AnyAttributeBuffer].name)
    assertResult("hihi")(firstHeaderAttribute.asInstanceOf[AnyAttributeBuffer].data)

  }

  var realMessage = """<?xml version="1.0" ?>
<env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope">
      <env:Body>
    		<TestBody>
    			<TestBodySub>Test</TestBodySub>
    		</TestBody>
    	</env:Body>
      <env:Header>
    		<ns1:Action xmlns:ns1="http://schemas.xmlsoap.org/ws/2004/08/addressing">{http://extoll.de/mex/protocol/speak}:SpeakRequest</ns1:Action>
      </env:Header>
</env:Envelope>
      
      
      """

  test("Streamin a real message") {

    // Create and print
    //---------------------

    AnyXList(classOf[Action])
    AnyXList(classOf[TestBody])

    var message = SOAPMessage(ByteBuffer.wrap(realMessage.getBytes))

    println("Real message example: " + message.toXMLString)

  }

  test("Streamin with Any") {

    var msg = """<?xml version="1.0" ?><env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope"><env:Header><ns1:Action xmlns:ns1="http://schemas.xmlsoap.org/ws/2004/08/addressing">fault</ns1:Action></env:Header><env:Body><env:Fault><Message>User rleys is unknown</Message></env:Fault></env:Body></env:Envelope>"""

  }

  test("JSON SOAP With mapped objects") {

    var jsonMessage = """
{
"Envelope" : {
	 
	  "Header" : {

	  },

	  "Body" : {
    		  
        "TestBody": {
    		 
           "TestBodySub" : "Test"
         }
	  }

 }
    
}
        """

    // Read
    //---------------
    AnyXList(classOf[Action])
    AnyXList(classOf[TestBody])
    var message = JSONSOAPMessage(jsonMessage)

    assert(message.header != null)
    assert(message.body != null)
    assert(message.body.content.size > 0)

    message.body.content.foreach {
      bc => println(s"Body Content: " + bc)
    }

  }

}
