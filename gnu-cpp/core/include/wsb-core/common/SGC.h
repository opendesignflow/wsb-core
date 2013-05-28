/*
 * SGC.h
 *
 *  Created on: Aug 23, 2011
 *      Author: rleys
 */

#ifndef SGC_H_
#define SGC_H_

// Includes
//--------------

//-- Std
#include <list>
using namespace std;

namespace OSI {
namespace WSB {


class Garbageable {


public:
	Garbageable();
	virtual ~Garbageable();
};


template <class T>
class GarbageableWrapper : public Garbageable {

private:
	T * reference;

public:
	GarbageableWrapper(T * ref) {
		this->reference = ref;
	}
	virtual ~GarbageableWrapper() {
		delete this->reference;
	}

	T * getReference() {
		return this->reference;
	}

};

template <class T>
class LGC : public Garbageable {

private:
	T * reference;

public:
	LGC(T * ref) {
		this->reference = ref;
	}
	virtual ~LGC() {
		delete this->reference;
	}

	T * getReference() {
		return this->reference;
	}

	T* operator->() {
		return this->reference;
	}


};


/**
 * Local SGC is designed to Autorelease resources that are created with pointers.
 *
 * Usage is following:
 *
 * xxx mymethod() {
 *
 * 	  LSGC lsgc;
 *
 * 	  MyType * s = new MyType();
 * 	  AnotherType * g = Builder::getInstance();
 *
 *
 *
 *   // After the method exists, LSGC gets destroyed, and propagates to all the registered pointers
 * }
 *
 *
 */
class LSGC {

protected:

	list<Garbageable*> gRefs;


public:
	LSGC();
	virtual ~LSGC();

	/**
	 * Registers the class to be garbaged
	 * @param
	 * @return
	 */
	template<class T = Garbageable> T* toGarbage(T * o) {
		this->gRefs.push_back(o);
		return o;
	}


};

#define _LSGC(pointer) lsgc.toGarbage(pointer)
#define _LSGCW(pointer) lsgc.toGarbage(new GarbageableWrapper<>(pointer))->getReference()

} /* namespace WSB */
} /* namespace OSI */

#endif /* SGC_H_ */
