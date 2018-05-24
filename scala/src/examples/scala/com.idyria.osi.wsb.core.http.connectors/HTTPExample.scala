/*
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2017 OpenDesignFlow.org
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
package com.idyria.osi.wsb.core.network.connectors.http
 
//import com.idyria.osi.wsb.core.network.connectors.http._
/*
import com.idyria.osi.wsb.core.message.http._
import com.idyria.osi.wsb.core.network.connectors.tcp._

import com.idyria.osi.wsb.core._

import scala.io._

import java.io._
import java.nio._

object HTTPExample extends App {

	println("Welcome to http example")

	// Create Engine
	//-----------------------
	var engine = new WSBEngine()

	var connector = HTTPConnector(57300)

	connector.on("server.started") {

		println("Server started HTTP ")

	}
	connector.on("server.accepted") {

		println("Connection accepted on HTTP ")

	}
	connector.onWith("http.connector.receive") {

		buffer : ByteBuffer =>

				println("Got Datas: "+new String(buffer.array))

	}
	connector.on("server.read") {
		println("Reading some datas ")
	}
	connector.on("server.read.datas") {
		println("Read some datas ")
	}
	connector.on("protocol.receive.endOfData") {
		println("Protocol Found a complete data set")
	}

	// Message Catcher
	//------------------------
	engine.localBus.registerClosure {
		msg : HTTPRequest => 
			println("Received AIB Message")

			  // Prepare Write
		      var channel = msg.networkContext.asInstanceOf[TCPNetworkContext].socket
		      var page = s"""
		      <html>
		      	<head>

		      	</head>
		      	<body>
		      		<h1>Hello World!!</h1>
		      	</body>
		      </html>
		      """

		      channel.write(ByteBuffer.wrap("HTTP/1.1 200\n".getBytes))
		      channel.write(ByteBuffer.wrap("Content-Type: text/html\n".getBytes))
		      channel.write(ByteBuffer.wrap(("Content-Length: "+page.getBytes().length+"\n").getBytes))
		      channel.write(ByteBuffer.wrap(page.getBytes))
		      channel.close
		      channel.socket.close
	}

	// Add
	//------------------------------
	engine.network.addConnector(connector)

	engine.lInit
	engine.lStart

	

	Source.stdin.getLines.foreach {
		f =>
	}

	engine.lStop

	println("End of example")

}
*/
