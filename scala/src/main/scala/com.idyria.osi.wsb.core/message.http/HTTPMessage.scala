/**
 *
 */
package com.idyria.osi.wsb.core.message.http

import com.idyria.osi.wsb.core.Logsource
import com.idyria.osi.wsb.core.message._

import java.nio._

/**

  Qualifier: s"http:$path:$operation"
  Example: http:/index:GET

 * @author rleys
 *
 */
class HTTPRequest (
    
      var operation : String,
      var path : String,
      var version : String
    
		)  extends Message {

	protected var parameters =  scala.collection.mutable.Map[String,String]()
  
	// Use Path as qualifier
  this.qualifier = s"http:$path:$operation"

	/**
	 * Record a parameter in internal parameter map
	 * Parameters are read from or written as lines like:
	 *  
	 *  NAME: VALUE
	 */		
	def addParameter(name : String, value:String) = parameters+=(name -> value)

  def toBytes = ByteBuffer.wrap(s"$operation $path HTTP/$version".getBytes)
}


object HTTPRequest extends MessageFactory with Logsource {
  
  def apply(data: Any) : HTTPRequest = {

      build(data.asInstanceOf[scala.collection.mutable.Buffer[String]])

  }

  /**
   * Create HTTPMessage
   * - 1st line: GET/PUT...  /path/ HTTPVERSION
   */
  def build(lines : scala.collection.mutable.Buffer[String]): HTTPRequest = {
    
    // Prepare regexps
    //----------------------
    var firstLineRegexp  = """(GET|POST|PUT) (.*) HTTP/([0-9]+\.[0-9]+)""".r
    var parameterLineRegexp = """([\w-])+: (.+)""".r
    
    
    // Parse First Line
    //-----------------------
    var firstLineRegexp(operation,path,version) = lines(0)
    var message = new HTTPRequest(operation,path,version)
    
    println("Got HTTP Message for path: "+message.path+" and operation "+operation)
     
    // Parse Parameters
    //-------------
    lines.drop(1).foreach {
      line =>
        
        println(s"Param: $line")
        
        try {
          var parameterLineRegexp(name,value) = line
          message.addParameter(name, value)
        } catch {
          case e : Throwable => println(s"Could not parse HTTP parameter $line")
        }
    }
    
    // Set Qualifier on message: Host:Port/pat
    //----------------
    if (message.parameters.contains("Host"))
      message.qualifier+=message.parameters("Host")
    message.qualifier+=message.path
    
    return message
    
  }
  
}

class HTTPResponse (
    
      var contentType : String,
      var content : String
    
    )  extends Message {

  var code = 200

  def toBytes : ByteBuffer = {

ByteBuffer.wrap(s"""HTTP/1.1 $code
Content-Type: $contentType
Content-Length: ${content.getBytes.length}
$content

""".getBytes)


  }

}
object HTTPResponse {

  def apply(contentType: String, content:String) : HTTPResponse = {


    new HTTPResponse(contentType,content)


  }

}
