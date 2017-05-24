

#include <engine/WSEngine.h>
using namespace OSI::WSB;

void engine_start() {

	//-- Will start an engine
	WSEngine * eng = new WSEngine();
	eng->init();
	eng->start();

}


