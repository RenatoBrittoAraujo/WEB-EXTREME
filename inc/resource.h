#ifndef RESOURCE
#define RESOURCE

#include "request.h"

class Resource
{
public:
  Resource() {}
  std::string index(Request request) { return ""; }
  std::string show(Request request) { return ""; }
  std::string destroy(Request request) { return ""; }
private:
};

#endif