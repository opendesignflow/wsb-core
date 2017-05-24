/*
 * test-comp-c.c
 *
 *  Created on: Nov 15, 2011
 *      Author: rleys
 */

#include <typeinfo>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <error.h>
#include <errno.h>
#include <cstdio>
#include <sys/param.h>
#include <sys/sysctl.h>





class Dobject  {

	string message;

public:
	Dobject(string message) {
		this->message = message;
	}



};

// Generate test to try things out
int main() {


	int intType = 0;



	cout << "Type of int: " << typeid(intType).name() << " <-> " ;




	return 0;

}

