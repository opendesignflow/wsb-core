/**
 * URI.h
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

#ifndef URI_H_
#define URI_H_

// Includes
//-----------------

//-- Std
#include <string>
using namespace std;

//-- Regexp
#include <regex.h>

// Regexp defines to help definition
//----------------

// Server based: [user-info@]host[:port]
#define URI_REGEXP_AUTHORITY "//(([a-zA-Z0-9_]+@)?(([a-zA-Z0-9]+\\.?)+)(:[0-9]+)?)?"

#define URI_REGEXP_PATH  "/?([#\\.a-zA-Z0-9_-]+/?)+"
#define URI_REGEXP_QUERY "\\?"

#define URI_CREATEREGEXP(buffer) sprintf(buffer,"([a-z]+):(%s)?(%s)?(.*)",URI_REGEXP_AUTHORITY,URI_REGEXP_PATH);

namespace OSI {
namespace WSB {

/**
 * A Utility class to parse a URI and get its different components
 * This is useful for example for Connectors to be able to build up connections
 * based on a simple string
 *
 * URI Spec, taken from Java documentation:
 *
 * [scheme:][//authority][path][?query][#fragment]
 *
 *
 */
class URI {

    protected:

        /// The Regular expression handle
        regex_t regexp;

        /// The URI we are analysing
        string uri;

        /** \defgroup parsing results */
        /** @{ */

        /// Was the URI valid
        bool    valid;

        /// The scheme
        string  scheme;

        /// Is absolute (did we found a //authority part ? )
        bool    absolute;

        /// Host : hostname or IP address
        string  host;

        /// User name declared to connect to host
        string  username;

        /// Host: port. default to 0
        int     port;

        /// Path part
        string path;

        /** @} */


    public:

        /**
         * Constructor preparses the URI
         * @param uri
         */
        URI(string uri);
        virtual ~URI();

        /**
         * Return this base URI
         * @return
         */
        string  getURI();

        /**
         * If the URI matched against the format
         * @return
         */
        bool    isValid();

        /**
         * The scheme
         * @return empty string if not found/defined
         */
        string  getScheme();

        /**
         * Is the URI is absolute
         * @return false per default
         */
        bool    isAbsolute();

        /**
         * The scheme
         * @return empty string if not found/defined
         */
        string  getHost();

        /**
         * The Username defined in host
         * @return empty string if not found/defined
         */
        string  getUserName();

        /**
         * The port defined in host
         * @return 0 if not found/defined
         */
        int     getPort();

        /**
         * The path part of the URI
         *
         * @return empty string if not found/defined
         */
        string  getPath();


};

} /* namespace WSB */
} /* namespace OSI */
#endif /* URI_H_ */
