#ifndef REQUEST
#define REQUEST

#include <string>

enum REQUEST_TYPE
{
  INVALID,
  GET,
  POST
};

class Request
{
public:
  // Parses request text
  Request(const std::string request);
  // Returns resource path of request
  std::string getPath();
  // Returns true if request involves favicon
  bool isFaviconReq();
  // Returns type of request (GET, POST)
  REQUEST_TYPE getType();
private:
  std::string request_text;
  REQUEST_TYPE type = INVALID;
  std::string path = "";
};

#endif