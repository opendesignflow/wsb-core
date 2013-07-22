package com.idyria.osi.wsb.core.network


import org.scalatest._

import com.idyria.osi.wsb.core.network.connectors.tcp._
import com.idyria.osi.wsb.core.network.connectors.simple._

class NetworkSuites extends Suites(
  new TCPConnectorSuite,
  new SimpleProtocolSuite
)
