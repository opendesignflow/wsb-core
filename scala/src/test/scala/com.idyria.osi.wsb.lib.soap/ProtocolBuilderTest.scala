package com.idyria.osi.wsb.lib.soap

import org.scalatest.FunSuite
import com.idyria.osi.ooxoo.model.out.scala.ScalaProducer
import com.idyria.osi.ooxoo.model.writers.StdoutWriter


/**
 * Provides extra functions to simplify model building
 */
class ProtocolBuilderTest extends FunSuite {
  
  test("Simple test") {
    
    var model = new ProtocolBuilder {
      

      message("TestMessage") {
        
        request {
          
          
          
        }
        
        response {
          
        }
        
      }
      
      message("PingPong") {
        
        request {
          
        }
        
        response {
          
          attribute("date") ofType "dateTime"
          
        }
        
      }
      
    }
    
    var scalaProducer = new ScalaProducer
    
    scalaProducer.produce(model, new StdoutWriter)
    
    /*model.topElements.foreach {
      te => 
        
        
        
        println(s"Element: "+te.)
        
    }*/
    
    
  }
  
}