#ifndef RESPONSE
#define RESPONSE

#include <string>

#include "request.h"

class Response
{
public:
  // Creates a response for given request
  Response(Request request);
  // Returns response text
  std::string getResponse();
  // Returns number of bytes in response
  int getResponseSize();
private:
  // Loads a file into a string
  std::string loadFile(const std::string path, bool raw = false);
  
  std::string response_text;
};

#endif