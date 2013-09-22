package com.idyria.osi.wsb.core.network

 

/**

    The NetworkContext Class is used by messages to refer back to the network layer they were send/from to

*/
class NetworkContext {



    /**
        Attachments map to store any kind of extra objects in the network context
    */
    var attachments = Map[String,Any]()

    /**
        The qualifier should uniquely identify the Context, so that connectors can initiate connections or send responses to the right client
    */
    var qualifier : String = null

    override def toString : String = qualifier match {
        case null => qualifier.toString
        case _ => qualifier
    }

}
