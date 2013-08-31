/**
 *
 */
package com.idyria.osi.wsb.core.message.http

import com.idyria.osi.wsb.core.message.mime._
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
    
		)  extends Message with MimePart {

  
	// Use Path as qualifier
  this.qualifier = s"http:$path:$operation"

	

  def toBytes = ByteBuffer.wrap(s"$operation $path HTTP/$version".getBytes)

  // URL Parameters
  //-------------------------

  /**
    If the content type matches application/x-www-form-urlencoded
    Then try to find in bytes string the whished parameter
  */
  def getURLParameter(name:String) : Option[String] = {

    this.parameters.get("Content-Type") match {
      case Some(contentType) if(contentType.matches("""\s*application/x-www-form-urlencoded.+""")) => 

          (name+"""=([\w%+_-]+)(?:&|$)""").r.findFirstMatchIn(new String(bytes)) match {

            // Found value for URL parameter, decode it:
            case Some(matched) => 
                Option(java.net.URLDecoder.decode(matched.group(1),"UTF-8"))
            case _ => None
          }


      case _ => None
    }
    

  } 


  // Multipart Data Support
  //--------------
  def isMultipart : Boolean = {

    this.parameters.get("Content-Type") match {

      case Some(contentType) if (contentType.matches("multipart/form-data.*")) => true
      case _ => false
    }

  }
}


object HTTPRequest extends MessageFactory with Logsource {
  
  def apply(data: Any) : HTTPRequest = {

      build(data.asInstanceOf[MimePart])

  }

  var lastFirstMessage : HTTPRequest = null

  /**
   * Create HTTPMessage
   * - 1st line: GET/PUT...  /path/ HTTPVERSION
   */
  def build(part : MimePart): HTTPRequest = {
    
    // Prepare regexps
    //----------------------
    var firstLineRegexp  = """(GET|POST|PUT) (.*) HTTP/([0-9]+\.[0-9]+)""".r
    var parameterLineRegexp = """([\w-])+: (.+)""".r
    
    

    // Parse First Line
    //-----------------------
    firstLineRegexp.findFirstMatchIn(part.protocolLines(0)) match {

      //-- Got First Message
      case Some(matched) =>

        println(s"[HTTP] -> First Message from part ${part.hashCode} with protocol line: "+part.protocolLines(0))
        lastFirstMessage = new HTTPRequest(matched.group(1),matched.group(2),matched.group(3))

        println("Got HTTP Message for path: "+lastFirstMessage.path+" and operation "+lastFirstMessage.operation)

        // Add Parameters
        //-------------
        lastFirstMessage(part)
      

      //-- Maybe a Continued Content in case of a multipart message
      case None if (lastFirstMessage!=null && lastFirstMessage.isMultipart)=> 

        println(s"[HTTP] -> Multipart element, create a request with the same path as previous message")
        var message = new HTTPRequest(lastFirstMessage.operation,lastFirstMessage.path,lastFirstMessage.version)
        message.bytes = part.bytes

        // Add Parameters
        //-------------
        message(part)

        return message

      //-- No Idea
      case _ => 
        println(s"[HTTP] -> Not a first message and not a multipart part")

    }

  
    return lastFirstMessage
    
  }
  
}

class HTTPResponse (
    
      var contentType : String,
      var content : ByteBuffer
    
    )  extends Message {

  var code = 200

  def toBytes : ByteBuffer = {

    var header = s"""HTTP/1.1 $code
Status: 200 OK
Content-Type: $contentType
Cache-Control: no-cache
Content-Length: ${content.capacity}

"""
    var res = ByteBuffer.allocate(header.getBytes.size+content.capacity)
    res.put(header.getBytes)
    res.put(content)
  
    if (contentType=="text/html") {
      println(s"Sending: "+new String(res.array))
    }
    //println(s"Sending: "+new String(res.array))

    res.flip
    res 

    

    //var res = ByteBuffer.wrap(resData.getBytes)
    //res.put(content)
    //res

  }

}
object HTTPResponse {

  def apply(contentType: String, content:ByteBuffer) : HTTPResponse = {


    new HTTPResponse(contentType,content)


  }

  def apply(contentType: String, content:String) : HTTPResponse = {


    new HTTPResponse(contentType,ByteBuffer.wrap(content.getBytes))


  }

}
