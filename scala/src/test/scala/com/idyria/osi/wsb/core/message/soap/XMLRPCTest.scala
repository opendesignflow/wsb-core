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
package com.idyria.osi.wsb.core.message.soap

import org.scalatest.FunSuite
import com.idyria.osi.ooxoo.core.buffers.datatypes.XSDStringBuffer

class XMLRPCTest extends FunSuite {

  test("Parse XML-RPC") {

    var basexml = <methodCall>
                    <methodName>test</methodName>
                    <params>
                      <param>
                        <value><i4>42</i4></value>
                      </param>
                      <param>
                        <value><int>42</int></value>
                      </param>
                      <param>
                        <value><boolean>true</boolean></value>
                      </param>
                      <param>
                        <value><double>4.0</double></value>
                      </param>
                      <param>
                        <value><string>Hello</string></value>
                      </param>
                      <param>
                        <value>Hello</value>
                      </param>
                      <param>
                        <value><base64>Hello</base64></value>
                      </param>
                    </params>
                  </methodCall>

    // parse
    //---------
    var mc = MethodCall(basexml.toString)

    // Checks
    //----------------
    expectResult("test")(mc.methodName.toString)

    // Params checks
    //-------------------
    expectResult(7)(mc.params.param.size)

    //-- Int
    expectResult(classOf[Int])(mc.params.param(0).value.toType)

    //-- Int
    expectResult(classOf[Int])(mc.params.param(1).value.toType)

    //-- boolean
    expectResult(classOf[Boolean])(mc.params.param(2).value.toType)

    //-- double
    expectResult(classOf[Double])(mc.params.param(3).value.toType)

    //-- string
    expectResult(classOf[String])(mc.params.param(4).value.toType)

    //-- string
    expectResult(classOf[String])(mc.params.param(5).value.toType)

    //-- Base64
    expectResult(classOf[Array[Byte]])(mc.params.param(6).value.toType)

    // All previous 7 tests in one line
    expectResult(true)(mc.params.param.map(m => m.value.toType) == List(classOf[Int], classOf[Int], classOf[Boolean], classOf[Double], classOf[String], classOf[String], classOf[Array[Byte]]))

  }

  test("Intermediary build") {

    var testIntermediary = new XMLRPCSoapHandler {
      
      def test1() : Unit = {
        
      }
      
      def test1(str: String) : Unit = {
        
      }
      
      def test2(id: Int) : Unit = {
        
      }
      
      
    }
    
    
  }

}