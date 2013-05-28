/**
 * IOUtils.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "IOUtils.h"

namespace OSI {
namespace WSB {


IOUtils IOUtils::singleton;

IOUtils::IOUtils() {
    // TODO Auto-generated constructor stub

}

IOUtils::~IOUtils() {
    // TODO Auto-generated destructor stub
}


long int IOUtils::readBytesFromFile(int fd,size_t targetLength,void * buffer) {

    //-- Prepare data
    size_t totalReadBytes = 0 ;
    long int readBytes = 0; // Status for each read call

    do {

        // Read
        readBytes = read(fd,buffer+totalReadBytes,targetLength-totalReadBytes);

        // Return if an error occured
        if (readBytes<0) {
            return readBytes;
        }
        // If 0 -> Return actual read size and stop
        else if (readBytes==0) {
           break;
        }
        // Otherwise -> actualize
        else {
            totalReadBytes += readBytes;
        }

    } while (totalReadBytes < targetLength);

    // Return read total bytes
    return totalReadBytes;


}

long int IOUtils::writeBytesToFile(int fd,size_t targetLength,const void * buffer) {

    //-- Prepare data
    size_t totalWrittenBytes = 0 ;
    long int writtenBytes = 0; // Status for each read call

    do {

        // Read
        writtenBytes = write(fd,buffer+totalWrittenBytes,targetLength-totalWrittenBytes);

        // Return if an error occured
        if (writtenBytes<0) {
            return writtenBytes;
        }
        // If 0 -> Return actual read size and stop
        else if (writtenBytes==0) {
           break;
        }
        // Otherwise -> actualize
        else {
            totalWrittenBytes += writtenBytes;
        }

    } while (totalWrittenBytes < targetLength);

    // Return read total bytes
    return totalWrittenBytes;

}

} /* namespace WSB */
} /* namespace OSI */
