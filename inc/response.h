#ifndef RESPONSE
#define RESPONSE

#include <string>

#include "request.h"

class Response
{
public:
  Response(Request request);
  std::string getResponse();
  int getResponseSize();
private:

};

#endif