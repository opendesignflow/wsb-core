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
    expectResult(true)(mc.params.param.map(m ⇒ m.value.toType) == List(classOf[Int], classOf[Int], classOf[Boolean], classOf[Double], classOf[String], classOf[String], classOf[Array[Byte]]))

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