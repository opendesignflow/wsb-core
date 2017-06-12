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
     
     assertResult(true)(str.matches(""".*Action.*>.*Test.*"""))
    
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
    assertResult(true)(actionHeader.isDefined)

    assertResult("Test")(actionHeader.get.toString)

  }

}
