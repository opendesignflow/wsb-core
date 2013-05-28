/**
 * URI.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <string>
#include <cstring>
#include <iostream>
#include <malloc.h>
using namespace std;

//-- Regexp
#include <re_comp.h>
#include <regex.h>

#include "URI.h"

namespace OSI {
namespace WSB {

URI::URI(string uri) {

    //-- Default to parsing results
    this->valid = false;
    this->absolute = false;



    //-- Record URI
    this->uri = uri;

    //-- Prepare URI Regexp
    char expr[2048];
    URI_CREATEREGEXP(expr)
    int errcode = regcomp(&(this->regexp),expr,REG_EXTENDED);
    if (errcode!=0) {
        size_t length = regerror (errcode, &(this->regexp), NULL, 0);
        char *buffer = (char*) calloc (length,sizeof(char));
        (void) regerror (errcode, &(this->regexp), buffer, length);

        cerr << "Error while compiling: "<< endl;
        cerr << "\t Expression: " << expr<< endl;
        cerr << "\t Result: " << buffer<< endl;

    }

    //-- Test
    size_t nmatch = 20;
    regmatch_t matchptr[nmatch];

    errcode = regexec(&(this->regexp),this->uri.c_str(),nmatch,matchptr,0);
    if (errcode!=0) {
        size_t length = regerror (errcode, &(this->regexp), NULL, 0);
        char *buffer = (char*) calloc (length,sizeof(char));
        (void) regerror (errcode, &(this->regexp), buffer, length);

        cerr << "Error while matching: " << endl;
        cerr << "\t Source: " << this->uri<< endl;
        cerr << "\t Expression: " << expr << endl;
        cerr << "\t Result: " << buffer<< endl;

    }


    //-- Extract the scheme
    this->valid = true;
    this->scheme = this->uri.substr(matchptr[1].rm_so,matchptr[1].rm_eo-matchptr[1].rm_so);

    //cout << "-Scheme: " << this->scheme << endl;

    // Continue analysis
    //-------------------------------

    //-- If the next group is valid we have an authority
    int np = 1;
    np++;
    string next;
    if (matchptr[np].rm_so!=-1) {

        //-- Get the authority
        next = this->uri.substr(matchptr[np].rm_so,matchptr[np].rm_eo-matchptr[np].rm_so);
        string authorityContent = next.length()>2 ? next.substr(2,next.length()-2):"";


        //cout << "Authority: " << authorityContent << endl;

        //-- Is absolute then
        this->absolute = true;

       if (authorityContent.length()>0) {

            //-- Look for host in authority
            //--------------------------------------

            //-- Look and separate parts (the next subexp is the entire authority content)
            np++;
            np++;

            //-- The first group could be the user
            if (matchptr[np].rm_so!=-1) {
                next = this->uri.substr(matchptr[np].rm_so,matchptr[np].rm_eo-matchptr[np].rm_so);
                this->username = next.substr(0,next.length()-1);

                //cout << "\t\tHost user: " << this->username << endl;
            }

            //-- Next is the host
            np++;
            this->host = this->uri.substr(matchptr[np].rm_so,matchptr[np].rm_eo-matchptr[np].rm_so);
            np++; // Jump over inside host group

            //cout << "\t\tHost:" << this->host << endl;


            //-- Next could be port
            np++;
            if (matchptr[np].rm_so!=-1) {

                // Remove ':' character in front of port
                next = this->uri.substr(matchptr[np].rm_so,matchptr[np].rm_eo-matchptr[np].rm_so);
                next = next.substr(1,next.length()-1);
                this->port = atoi(next.c_str());

                //cout << "\t\tPort: " << this->port << endl;
            } else {
                this->port = 0;
            }

        } else {

            // Jump over all non matched groups in authority
            np+=5;

        } // EOF if authority content

    } else {
        // Jump over all non matched groups in authority
        np+=5;
    }

    //-- After that, there coud be a path
    np++;
    //cout << "Looking for path at np: " << np << endl;
    if (matchptr[np].rm_so!=-1) {

        next = this->uri.substr(matchptr[np].rm_so,matchptr[np].rm_eo-matchptr[np].rm_so);

        cout << "Path: " << next << endl;


    }

//    for (int i= 0; i<nmatch;i++) {
//        if (matchptr[i].rm_so!=-1) {
//
//           // cout << "Match["<< i <<"]: "<< this->uri.substr(matchptr[i].rm_so,matchptr[i].rm_eo-matchptr[i].rm_so) << endl;
//
//        }
//    }




    regfree(&(this->regexp));


}

URI::~URI() {
    // TODO Auto-generated destructor stub
}

string  URI::getURI() {
    return this->uri;
}
bool    URI::isValid() {
    return this->valid;
}

string  URI::getScheme() {
    return this->scheme;
}
bool    URI::isAbsolute() {
    return this->absolute;
}
string  URI::getHost() {
    return this->host;
}
string  URI::getUserName() {
    return this->username;
}
int     URI::getPort() {
    return this->port;
}

string  URI::getPath() {
    return this->path;
}

} /* namespace WSB */
} /* namespace OSI */
