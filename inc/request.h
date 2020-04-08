#ifndef REQUEST
#define REQUEST

#include <string>

#include <map>

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

const std::map<RESPONSE_STATUS, std::string>
  response_status_string =
{
  {RESPONSE_STATUS::OK, "OK"},
  {RESPONSE_STATUS::CREATED, "CREATED"},
  {RESPONSE_STATUS::NO_CONTENT, "NO_CONTENT"},
  {RESPONSE_STATUS::NOT_FOUND, "NOT_FOUND"},
  {RESPONSE_STATUS::SEE_OTHER, "SEE_OTHER"},
  {RESPONSE_STATUS::SERVER_ERROR, "SERVER_ERROR"},
  {RESPONSE_STATUS::UNAUTHORIZED, "UNAUTHORIZED"},
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