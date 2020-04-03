#ifndef REQUEST
#define REQUEST

#include <string>

class Request
{
public:
  Request(const char* response_buffer);
private:
  std::string request_text;
};

#endif