#ifndef REQUEST
#define REQUEST

#include <string>

enum REQUEST_TYPE
{
  ANY,
  GET,
  POST,
  PUT,
  DELETE
};

enum RESPONSE_STATUS
{
  OK = 200,
  CREATED = 201,
  NO_CONTENT = 204,
  SEE_OTHER = 303,
  UNAUTHORIZED = 403,
  NOT_FOUND = 404,
  SERVER_ERROR = 505
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
  REQUEST_TYPE type = ANY;
  std::string path = "";
};

#endif