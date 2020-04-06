#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>

#include "request.h"
#include "resource.h"

enum RESPONSE_TYPE
{
  STATIC,
  DINAMIC
};

class Route
{
public:
  Route(std::string file, bool raw_format) :
    file("assets/" + file), raw_format(raw_format)
  {
    response_type = RESPONSE_TYPE::STATIC;
    request_type = REQUEST_TYPE::GET;
  }
  Route(Resource resource, REQUEST_TYPE request_type) :
    resource(resource), request_type(request_type)
  {
    response_type = RESPONSE_TYPE::DINAMIC;
  }
  std::string parseResponse();
private:
  REQUEST_TYPE request_type;
  RESPONSE_TYPE response_type;
  std::string file;
  bool raw_format;
  Resource resource;
};

std::map<std::string, Route> router =
{
  {"/", Route("about.html", false)},
  {"/favicon.ico", Route("favicon.ico", true)},
  {"/posts/new", Route("createPost.html", false)}
};

class Response
{
public:
  // Creates a response for given request
  Response(Request request);
  // Returns response text
  std::string getResponse();
  // Returns number of bytes in response
  int getResponseSize();
  // Response stataus code
  int getStatusCode();
  // Response data
  std::string getData();
  // Response header
  std::string getHeader();
private:
  std::string header;
  std::string data;

  // Loads a file into a string
  std::string loadFile(const std::string path, bool raw = false);
  std::string response_text;
};

#endif