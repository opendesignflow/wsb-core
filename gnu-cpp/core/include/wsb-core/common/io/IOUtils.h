/**
 * IOUtils.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef IOUTILS_H_
#define IOUTILS_H_

// Includes
//-----------------

namespace OSI {
namespace WSB {

class IOUtils {

    private:

        static IOUtils singleton;

    public:
        IOUtils();
        virtual ~IOUtils();


        /**
         * Reads from fd descriptor up to the targetLength bytes, maintaining a readBytes counter
         * to ensure continuing read call if the read function returns with a read length < to the targetLength
         *
         * @param fd
         * @param targetLength
         * @param buffer
         * @return -1 if an error occured, or the number of read bytes
         */
        static long int readBytesFromFile(int fd,size_t targetLength, void * buffer);

        /**
         * Writes to fd descriptor up to the targetLength bytes, maintaining a writtenBytes counter
         * to ensure continuing write calls if the write function returns with a write length < to the targetLength
         *
         * @param fd
         * @param targetLength
         * @param buffer
         * @return -1 if an error occured, or the number of written bytes
         */
        static long int writeBytesToFile(int fd,size_t targetLength,const void * buffer);

};


} /* namespace WSB */
} /* namespace OSI */

#endif /* IOUTILS_H_ */
