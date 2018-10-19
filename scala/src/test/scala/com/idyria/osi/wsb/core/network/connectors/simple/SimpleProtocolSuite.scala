package com.idyria.osi.wsb.core.network.protocols.simple


import org.scalatest._
import com.idyria.osi.wsb.core.network._
import com.idyria.osi.wsb.core.network.protocols._
import java.nio._
import java.io._
import java.util.concurrent._
import com.idyria.osi.wsb.core.network.NetworkContext
import com.idyria.osi.wsb.core.network.connectors.AbstractConnector

@Ignore
class SimpleProtocolSuite extends FunSuite  with GivenWhenThen {


    // Prepare Message and result
    //---------------
    var sendMessage = """<Envelope><Body></Body></Envelope>"""
    var expectedResult=s"Content-Length: ${sendMessage.getBytes.length}\n$sendMessage"
    var byteChunk = 2

    var messages : Map[String,String] = List(
        """<Envelope><Body></Body></Envelope>""",
        """<Envelope><Body></Body></Envelope>"""
        ).map { m => m -> s"""Content-Length: ${m.getBytes.length}\n$sendMessage""" }.toMap


    println(s"Messages input vector has ${messages.size} elements" )

    test("Send Standalone") {

        Given("A Client Protocol Handler")
        //-------------
 
        var clientHandler = new SimpleProtocolHandler(new NetworkContext)


        When("Sending on the client handler")
        //------------------
        var sendResult = clientHandler.send(ByteBuffer.wrap(sendMessage.getBytes),null)

        // Convert to string:
        println("Result: "+new String(sendResult.array()))

        Then("Output must match protocol")
        assertResult(expectedResult)(new String(sendResult.array()))



    }



    test("Receive Standalone one message in chuncks") {

        var toSend = ByteBuffer.wrap(expectedResult.getBytes)

        Given("A Server Protocol Handler")
        //-------------
        var  serverHandler = new SimpleProtocolHandler(new NetworkContext)

        serverHandler.onWith("start") {
            b : ByteBuffer =>
           // println(s"--------- Starting to analyse buffer with: ${b.remaining} elements -----------")
        }

        serverHandler.onWith("contentLength.foundFirstLine") {
            line : String =>
            println(s"--------- FOUND CONTENT LENGTH LINE: ${line} -----------")
        }

        serverHandler.onWith("contentLength.buffering") {
            c : Byte =>
            //println(s"--------- Content length buffering: ${c.asInstanceOf[Char]} -----------")
        }

         When("Receiving in chumps on the server handler")
        //---------------
        var currentPos = 0
        var resultBytes =new ByteArrayOutputStream()

        var lastResult = false
        toSend.limit(toSend.capacity)
        while (toSend.remaining > 0) {

            // Available bytes
            var bytesToRead = byteChunk
            if (bytesToRead>toSend.remaining) {
                bytesToRead = toSend.remaining
            }
            var readBytes = new Array[Byte](bytesToRead)

            //println("Bytes to read: "+bytesToRead+" with remaining: "+toSend.remaining )

            // Read
            toSend.get(readBytes)
            lastResult = serverHandler.receive(ByteBuffer.wrap(readBytes))


        }

        Then("Last returned boolean must be true")
        //---------------
        assertResult(true)(lastResult)

        And("The Produced Data must be the message")
        //------------------------------
        assertResult(sendMessage)(new String(serverHandler.availableDatas.head.array()))

    }



    test("Receive Standalone one message in one pass") {

        var toSend = ByteBuffer.wrap(expectedResult.getBytes)

        Given("A Server Protocol Handler")
        //-------------
        var  serverHandler = new SimpleProtocolHandler(new NetworkContext)

        serverHandler.onWith("start") {
            b : ByteBuffer =>
           // println(s"--------- Starting to analyse buffer with: ${b.remaining} elements -----------")
        }

        serverHandler.onWith("contentLength.foundFirstLine") {
            line : String =>
            println(s"--------- FOUND CONTENT LENGTH LINE: ${line} -----------")
        }

        serverHandler.onWith("contentLength.buffering") {
            c : Byte =>
            //println(s"--------- Content length buffering: ${c.asInstanceOf[Char]} -----------")
        }

         When("Receiving in one pass on the server handler")
        //---------------
        var lastResult = serverHandler.receive(toSend)


        Then("Last returned boolean must be true")
        //---------------
        assertResult(true)(lastResult)
        assertResult(1)(serverHandler.availableDatas.size)

        And("The Produced Data must be the message")
        //------------------------------
        assertResult(sendMessage)(new String(serverHandler.availableDatas.head.array()))


    }

    test("Receive Standalone two messages in one pass") {

        Given("A Server Protocol Handler")
        //-------------
        var  serverHandler = new SimpleProtocolHandler(new NetworkContext)

        When("Receiving in one pass on the server handler")
        //---------------
        var toSend = ByteBuffer.wrap(expectedResult.getBytes)
        var lastResult = serverHandler.receive(toSend)

        Then("The Produced Data must be the message")
        //----------------
        assertResult(sendMessage)(new String(serverHandler.availableDatas.head.array()))

        When("Doing the same again")
        //----------------

        toSend = ByteBuffer.wrap(expectedResult.getBytes)
        lastResult = serverHandler.receive(toSend)

        Then("there are two available messages")
        //--------------------
        assertResult(2)(serverHandler.availableDatas.size)
        assertResult(sendMessage)(new String(serverHandler.availableDatas.last.array()))
    }

   /* messages.foreach {
        case (message , protocolMessage) => println(s"OO Message as input source: "+message)
    }*/

    /*messages.foreach {
       case (message , protocolMessage) =>

        info(s"Message as input source: "+message)

        test("Receive Standalone on list of message:") {

        }
    }*/


   test("Client to server send") {


        Given("A client Simple TCP Connector")
        //-----------------------------------------
        var client = new SimpleMessageTCPConnector
        client.direction = AbstractConnector.Direction.Client

        Given("A server Simple TCP Connector")
        //-----------------------------------------
        var clientConnected = new Semaphore(0);

/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2017 OpenDesignFlow.org
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * #L%
 */
        var server = new SimpleMessageTCPConnector
        server.on("server.accepted") {
            println("Server accepted connection")
            clientConnected.release()
        }

        Then("Start")
        //---------------------
        server.cycleToStart
        assert(server.started.tryAcquire(1,TimeUnit.SECONDS) == true,"Server Started")


        client.cycleToStart
        assert(client.started.tryAcquire(1,TimeUnit.SECONDS) == true,"Client Started")


        // (wait for connection)
        clientConnected.acquire
        assertResult(1,"Server has client context map")(server.clientsContextsMap.size)

        When("Sending a simple XML message through client")
        //--------------------
        client.send(ByteBuffer.wrap(sendMessage.getBytes),null)

        Then("The message should come out of the handler")
        //--------------------------
        Thread.sleep(1000)

        // Get client context on server side to find back handler
        var phandler : ProtocolHandler[ByteBuffer] = ProtocolHandler(server.clientsContextsMap.head._2)
        assert(phandler!=null)
        assertResult(1)(phandler.availableDatas.size)

        assertResult(sendMessage)(new String(phandler.availableDatas.head.array()))

        // Stop
        //----------------------
        client.cycleToStop
        server.cycleToStop

    }

    


}
