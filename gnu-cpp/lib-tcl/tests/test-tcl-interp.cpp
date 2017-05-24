/*
 * test-tcl-interp.c
 *
 *  Created on: Nov 15, 2011
 *      Author: rleys
 */

// Includes
//-----------------


//-- TCL
#include <tcl/tcl.h>

/**
 *Try the TCL C interface
 * @return
 */
int main() {


	//-- Create an Interpreter
	Tcl_Interp * interpreter = Tcl_CreateInterp();

	Tcl_Eval(interpreter, "puts AAHAHA");

	return 0;

}
