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

class SOAPMessageTest extends FunSuite with GivenWhenThen {


    @xelement(name="TestHeader")
    class TestHeader extends ElementBuffer {
    }

    @xelement(name="TestHeader2")
    class TestHeader2 extends ElementBuffer {
    }

    @xelement(name="TestHeaderNS1",ns="ws-core:test")
    class TestHeaderNS1 extends ElementBuffer {
    }

    test("Create and serialize an empty messsage") {

        // Create and print
        //---------------------

        var message = SOAPMessage()

        println(message.toXMLString)


    }

    var messageWithHeaders : String = null
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

        expectResult(2)(message.header.content.size)

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

        expectResult(2)(message.header.content.size)

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
        expectResult(2)(message.header.content.size)

        // Check names
        //------------------
        expectResult("TestHeader")(message.header.content.head.asInstanceOf[AnyElementBuffer].name)
        expectResult("TestHeader2")(message.header.content.last.asInstanceOf[AnyElementBuffer].name)

    }

    var messageWithHeadersComplexer = """
    <env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope">
        <env:Header>
            <TestHeader name="hihi"></TestHeader>
            <TestHeader2></TestHeader2>
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
        expectResult(3)(message.header.content.size)
        expectResult(4)(message.body.content.size)

        // Check names
        //------------------
        expectResult("TestHeader")(message.header.content.head.asInstanceOf[AnyElementBuffer].name)
        expectResult("TestHeader2")(message.header.content.last.asInstanceOf[AnyElementBuffer].name)

        expectResult("TestHeader")(message.body.content.head.asInstanceOf[AnyElementBuffer].name)
        expectResult("TestHeader2")(message.body.content.last.asInstanceOf[AnyElementBuffer].name)

        // Check Attributes
        //----------------------
        var firstHeader = message.header.content.head.asInstanceOf[AnyElementBuffer]
        expectResult(1)(firstHeader.content.size)

        var firstHeaderAttribute = firstHeader.content.head
        expectResult(true)(firstHeaderAttribute.isInstanceOf[AnyAttributeBuffer])
        expectResult("name")(firstHeaderAttribute.asInstanceOf[AnyAttributeBuffer].name)
        expectResult("hihi")(firstHeaderAttribute.asInstanceOf[AnyAttributeBuffer].data)

    }

    
    
}
