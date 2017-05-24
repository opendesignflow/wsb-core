package stupidsocket


import org.scalatest._

import scala.io.Source
import javax.net.ServerSocketFactory
import java.net.InetSocketAddress
import java.io.PrintStream
import java.nio.channels._
import java.nio._

import com.idyria.osi.wsb.core.network.connectors.tcp._

import java.util.concurrent._

import java.net._

class StupidSocketTry extends FlatSpec with GivenWhenThen {
    
    var address = "localhost"
    var port = 9898



    "TCPConnector start and close" should "let same port resusable" in {

 
         Given("A started connector")
            //---------------------
            var connector = new TCPConnector() {
                port=9898

                // Dummy
                def protocolReceiveData( buffer : ByteBuffer, context: TCPNetworkContext) : Option[Iterable[Any]] = {
                    None
                }

                // Dummy
                def protocolSendData(buffer : ByteBuffer,context: TCPNetworkContext)   : ByteBuffer= {
                    null
                }

                this.on("server.started") {
                    println("Server reaching start")
                }

                this.on("server.end") {
                    println("Server reaching end")
                }


            }


            //-- Start It
            connector.cycleToInit
            connector.cycleToStart

            assert(connector.started.tryAcquire(1,TimeUnit.SECONDS) == true)

            Then("it can be stoped and cleaned")
            //----------------------

            //-- Stop It
            connector.cycleToStop

            // Check
            //--------------

            // Thread must die
            try {
                connector.getThread.join(1000)
            } catch {
                case e : Throwable => {
                    assert(false)
                }
            }

            // Check states
            //expectResult(false)(connector.serverSocket.socket.isBound)

            And("Port is resusable")
            println("Rebinding")
            var serverSocket = ServerSocketChannel.open();

/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2014 OSI / Computer Architecture Group @ Uni. Heidelberg
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/gpl-3.0.html>.
 * #L%
 */
            serverSocket.bind(new InetSocketAddress(address,port))
            serverSocket.close
    }


/*
 "Bind a socket and close it" should "let same port reusable" in {

        

        // Open
        var serverSocket = ServerSocketChannel.open();
        serverSocket.bind(new InetSocketAddress(address,port))

    
        // Close
        serverSocket.close
        //Thread.currentThread.wait(500)
        //expectResult(false)(serverSocket.socket.isBound)

        // Reopen
        serverSocket = ServerSocketChannel.open();
        serverSocket.bind(new InetSocketAddress(address,port))

        // Close
        serverSocket.close

 }

 "Bind a socket With a selector and close it" should "let same port reusable" in {



        // Open
        var serverSocket = ServerSocketChannel.open();
        serverSocket.bind(new InetSocketAddress(address,port))

        var selector = Selector.open()
        serverSocket.configureBlocking(false);
        serverSocket.register(selector,SelectionKey.OP_ACCEPT)

        // Close
        serverSocket.close

        // Reopen
        serverSocket = ServerSocketChannel.open();
        serverSocket.bind(new InetSocketAddress(address,port))

        // Close
        serverSocket.close

  

 }

 "Bind a socket and don't close it" should "not let same port reusable" in {

        // Open
        var serverSocket = ServerSocketChannel.open();
        serverSocket.bind(new InetSocketAddress(address,port))

        // Close
        //serverSocket.close

        // Reopen
        intercept[java.net.BindException] {
            var serverSocket2 = ServerSocketChannel.open();
            serverSocket2.bind(new InetSocketAddress(address,port))
        }
        
      
        // Close
        serverSocket.close

    
 }
*/


}
