/**
 *
 */
package com.idyria.osi.wsb.core.message

import com.idyria.osi.wsb.core.Logsource


/**
 * @author rleys
 *
 */
class HTTPMessage (
    
    var operation : String,
	var path : String,
	var version : String
    
		)  extends Message {

	protected var parameters =  scala.collection.mutable.Map[String,String]()
  
	// Use Path as qualifier
  
	/**
	 * Record a parameter in internal parameter map
	 * Parameters are read from or written as lines like:
	 *  
	 *  NAME: VALUE
	 */		
	def addParameter(name : String, value:String) = parameters+=(name -> value)
}
object HTTPMessage extends Logsource {
  
  /**
   * Create HTTPMessage
   * - 1st line: GET/PUT...  /path/ HTTPVERSION
   */
  def build(lines : List[String]): HTTPMessage = {
    
    // Prepare regexps
    //----------------------
    var firstLineRegexp  = """(GET|POST|PUT) (.*) HTTP/([0-9]+\.[0-9]+)""".r
    var parameterLineRegexp = """([\w-])+: (.+)""".r
    
    
    // Parse First Line
    //-----------------------
    var firstLineRegexp(operation,path,version) = lines(0)
    var message = new HTTPMessage(operation,path,version)
    
    println("Got HTTP Message for path: "+message.path)
    
    // Parse Parameters
    //-------------
    lines.drop(1).foreach {
      line =>
        
        println(s"Param: $line")
        
        try {
        var parameterLineRegexp(name,value) = line
        message.addParameter(name, value)
        } catch {
          case e : Throwable => logInfo(s"Could not parse HTTP parameter $line")
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