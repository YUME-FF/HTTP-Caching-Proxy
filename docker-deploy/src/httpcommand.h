#ifndef __HTTPCOMMAND__H__
#define __HTTPCOMMAND__H__
#include <boost/regex.hpp>

#include "helper.h"
#include "myexception.h"

class httpcommand {
   public:
    std::string request;
    std::string method;
    std::string port;
    std::string host;
    std::string url;
    std::string ifNoneMatch;
    std::string ifModifiedSince;
    bool isBadRequest;
    int maxStale;

   public:
    httpcommand();
    httpcommand(std::string req);
    void printRequestInfo();
    bool checkBadRequest(int client_fd, int thread_id);
    

   private:
    void parseMethod();
    void parseHostPort();
    void parseURL();
    void parseValidInfo();
    void parseMaxStale();
};

#endif