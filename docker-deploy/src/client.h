#ifndef __CLIENT__H__
#define __CLIENT__H__
#include "helper.h"

class Client {
 public:
  int client_fd;

  void initClientfd(const char * hostname, const char * port);
};
#endif