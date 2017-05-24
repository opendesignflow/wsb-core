/**
 * VDOMNode.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------
#include "VDOMNode.h"

namespace VDOM {

VDOMNode::VDOMNode() {


}

VDOMNode::~VDOMNode() {
    // TODO Auto-generated destructor stub
}



void VDOMNode::setUserData(string key,void * dataPointer) {

    this->userData[key] = dataPointer;

}


void * VDOMNode::getUserData(string key) {

    if (this->userData.count(key)==0)
        return NULL;
    else
        return this->userData[key];

}



} /* namespace VDOM */
