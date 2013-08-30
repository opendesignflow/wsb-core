package com.idyria.osi.wsb.core.message


import java.nio._

/**
    This class and subtypes are just classes that hold some data without any specific protocol informations

*/
abstract class DataMessage( var bytes : ByteBuffer) extends Message {


    def toBytes : ByteBuffer = bytes

}

class StringDataMessage(var content: String) extends DataMessage(ByteBuffer.wrap(content.getBytes)) {


}
