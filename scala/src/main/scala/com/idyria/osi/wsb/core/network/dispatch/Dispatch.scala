/*-
 * #%L
 * WSB Core
 * %%
 * Copyright (C) 2008 - 2017 OpenDesignFlow.org
								Richard Leys (leys dot richard at gmail):
								2008-2014 University of Heidelberg (Computer Architecture group)
								2008-2014 Extoll GmbH (extoll.de)
								2014-2017 University of Karlsruhe (KIT) - ASIC and Detector Lab
 * %%
 * This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * #L%
 */
package com.idyria.osi.wsb.core.network.dispatch

import com.idyria.osi.wsb.core.broker.MessageBroker
import com.idyria.osi.wsb.core.message.Message
import scala.reflect._

trait Dispatch {
  
  /**
   * Ensures the Provided message is passed to the message broker
   */
  def deliver(m:Message,b:MessageBroker)  : Unit
  
  def lstop : Unit
}


trait TypedDispatch[MT <: Message] extends Dispatch  {
  
  val ttag : ClassTag[MT] 
  
   def deliver(m:Message,b:MessageBroker)  : Unit = {
     //classTag[MT]
     ttag.runtimeClass.isInstance(m) match {
       case true => deliverMessage(m.asInstanceOf[MT],b)
       case false => 
     }
   }
   
   def deliverMessage(m:MT,b:MessageBroker) : Unit
}