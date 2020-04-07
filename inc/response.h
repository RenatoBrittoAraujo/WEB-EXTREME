#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>

#include "request.h"
#include "helpers.h"
#include "resource.h"

enum RESPONSE_TYPE
{
  STATIC,
  DINAMIC
};

enum FILE_TYPE
{
  HTML,
  IMAGE
};

struct Route
{
  Route() {}
  Route(std::string file) :
    file("assets/" + file)
  {
    response_type = RESPONSE_TYPE::STATIC;
    request_type = REQUEST_TYPE::GET;
    auto type = split(file, ".")[1];
    if (type == "html") file_type = FILE_TYPE::HTML;
    else file_type = FILE_TYPE::IMAGE;
  }
  Route(Resource *resource) :
    resource(resource)
  {
    response_type = RESPONSE_TYPE::DINAMIC;
  }
  REQUEST_TYPE request_type;
  RESPONSE_TYPE response_type;
  Resource *resource;
  std::string file;
  FILE_TYPE file_type;
  std::string path;
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

  void notFound();

  void htmlResponse(std::string file);
  void imageResponse(std::string file);
};

#endif