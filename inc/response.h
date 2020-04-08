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
  RESPONSE_STATUS response_status;

  // Sets response as a standard 404 not found page
  void notFound();

  // Add headers to response
  void buildHeaders();
  // Searches for assets, routes and resources to anwser request
  void findRequestResponse(Request request);
  // Craft response that redirect user to given url
  void redirectResponse(std::string redirect_to);
  // Returns a file as response
  void fileResponse(std::string file);
  // Handles a resource response
  void resourceReponse(Route route, Request req);
  // Parses and returns a valid query for a resource in server
  void assetResponse(std::string asset);
  // Returns true if file (or file path) is an image
  bool isImage(std::string file);
  // Adds an html link to response to stylesheets or scripts
  void addHTMLHeader(std::string header);
  // Returns true if string data is an html file
  bool isHTML(std::string data);

  std::string contentType;
  
  static const std::map<std::string, std::string> fileToContentType;
  static const std::set<std::string> imageExtensions;
};

#endif