#ifndef REQUEST
#define REQUEST

#include <string>
#include <vector>
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
  {RESPONSE_STATUS::NO_CONTENT, "NO CONTENT"},
  {RESPONSE_STATUS::NOT_FOUND, "NOT FOUND"},
  {RESPONSE_STATUS::SEE_OTHER, "SEE OTHER"},
  {RESPONSE_STATUS::SERVER_ERROR, "SERVER ERROR"},
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
  // Returns type of request
  REQUEST_TYPE getType();
  // Returns a vector of string, each string a header of request
  std::vector<std::string> getHeaders();
  // Returns the content of request
  std::string getData();
  // Retuns data for key in header
  std::string getHeaderValue(std::string key);
  // Retuns true if header contains some key
  bool headerHasKey(std::string key);
private:
  std::map<std::string, std::string> parsedHeaders;
  std::string request_text;
  REQUEST_TYPE type = ANY;
  std::string path = "";
  std::vector<std::string> headers;
  std::string data;
};

#endif