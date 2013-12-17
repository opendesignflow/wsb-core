package com.idyria.osi.wsb.core.message.soap

import com.idyria.osi.ooxoo.model.out.scala._
import com.idyria.osi.ooxoo.model.producers
import com.idyria.osi.ooxoo.model.producer
import com.idyria.osi.ooxoo.model.ModelBuilder
import com.idyria.osi.ooxoo.core.buffers.datatypes.CDataBuffer

@producers(Array(
    new producer(value=classOf[ScalaProducer])
))
object SOAP extends ModelBuilder {

    name="SOAPModel"
    
    namespace("env" -> "http://www.w3.org/2003/05/soap-envelope")
    parameter("scalaProducer.targetPackage" -> "com.idyria.osi.wsb.core.message.soap")


    "env:Envelope" is {


        "env:Header" is {

           any

        }

        "env:Body" is {

            any

        }

    }
    
    "env:Fault" is {
        
        "env:Code" is {
            
            "env:Value" enum("VersionMismatch","MustUnderstand","DataEncodingUnknown","Sender","Receiver")
            
            "env:Subcode" is {
            
                "env:Value" ofType "string"
                
                "env:Subcode" is {
                }
            }
        } 
        
        "env:Reason" is {
            //"env:Text" ofType "string"
          "env:Text" is {
            classType(classOf[CDataBuffer].getCanonicalName())
            
          }
        }
        
        // Optional
        //------------
        "env:Node" ofType "uri"
        "env:Role" ofType "uri"
        
        "env:Detail" is {
            
            any  
        
        } 
        
    
        
    }

}
