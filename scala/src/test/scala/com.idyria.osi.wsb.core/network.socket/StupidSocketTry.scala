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
                def protocolReceiveData( buffer : ByteBuffer, context: TCPNetworkContext) = {

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
                connector.join(1000)
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
