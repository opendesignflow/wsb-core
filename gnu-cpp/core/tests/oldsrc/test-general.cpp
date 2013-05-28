/*
 * test-comp-c.c
 *
 *  Created on: Nov 15, 2011
 *      Author: rleys
 */

// Includes
//-----------------

//-- Std
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <error.h>
#include <errno.h>
#include <cstdio>
#include <sys/param.h>
#include <sys/sysctl.h>

//-- URI
#include <common/URI.h>

//-- Dispatch
#include <common/dispatch/ParallelDispatchQueue.h>
#include <common/dispatch/Dispatchable.h>
using namespace EUVNI::WSB;

pthread_mutex_t countMutex = PTHREAD_MUTEX_INITIALIZER;
long count = 0;

class Dobject : public Dispatchable {

	string message;

public:
	Dobject(string message) {
		this->message = message;
	}

	virtual void run() {
		MutexLocker lock(&countMutex);
		//cout << "Hello I am " << message << endl;
		count++;
	}

};

// Generate test to try things out
int main() {
/*

	// Test Dispatch
	//-----------------------

	//-- Get CPU info

	//-- Create Queue
	ParallelDispatchQueue * queue = new ParallelDispatchQueue();
	queue->start();

	//-- Submit jobs
	for (int i = 0 ; i < 1000 ; i++) {
		stringstream name;
		name << "Number " << i;
		Dobject * a = new Dobject(name.str());
		a->dispatch(queue);
	}


	Dispatchable * barrier = queue->barrier();
	barrier->waitFinished();


	cout << "Finished: "<< count << endl;;



	cout << "*I: Press any key to exit" << endl;
	char c;
	cin >> c;

	queue->stop();

*/


    /* Test Some URIS */
    URI tcpIPPortURI("tcp://127.0.0.1:8087");
    URI tcpIPUserPortURI("tcp://rleys@127.0.0.1:8087");
    URI tcpHostUserPortURI("tcp://rleys@localhost:8087");
    URI tcpHostUserURI("tcp://rleys@localhost");

    URI tcpUnixImplicitURI("tcp://#wsb");
    URI tcpUnixFileURI("tcp:///var/run/wsb");

    URI loopback("loopback:");


	return 0;

}

