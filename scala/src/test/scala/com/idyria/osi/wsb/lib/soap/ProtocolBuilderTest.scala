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