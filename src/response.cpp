#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "response.h"
#include "request.h"
#include "helpers.h"
#include "resource.h"

/* ROUTING */

#include "posts.h"

std::map<std::pair<std::string, REQUEST_TYPE>, Route> router =
{
  {{"/", REQUEST_TYPE::GET}, Route("about.html")},
  {{"/favicon.ico", REQUEST_TYPE::GET}, Route("favicon.ico")},
  {{"posts", REQUEST_TYPE::ANY}, Route(new Posts())}
};

/* END OF ROUTING */

Response::Response(Request req)
{
  std::pair<std::string, REQUEST_TYPE> 
    requestRoute = { req.getPath(), req.getType() };
  Route route;
  if (not router.count(requestRoute))
  {
    auto resource = split(req.getPath(), "/")[0];
    if (not router.count({ resource, REQUEST_TYPE::ANY}))
    {
      notFound();
      return;
    }
    else
      route = router[{resource, REQUEST_TYPE::ANY}];
  }
  else
    route = router[requestRoute];
  if (route.response_type == RESPONSE_TYPE::STATIC)
  {
    if (route.file_type == FILE_TYPE::HTML)
      htmlResponse(route.file);
    else if (route.file_type == FILE_TYPE::IMAGE)
      imageResponse(route.file);
  }
  else
  {
    std::cout<<"HANDLING AND ASSEMBLING PAGE"<<std::endl;
    auto res = route.resource->handle(req);
    std::cout << "HANDLING AND ASSEMBLING PAGE DONE" << std::endl;
    delete route.resource;
    if (res.response_status == RESPONSE_STATUS::NOT_FOUND)
    {
      notFound();
      return;
    }
    this->data = res.data;
    this->header += "HTTP/1.1 200 OK\r\n";
    if (not res.additional_headers.empty())
      this->header += res.additional_headers;
  }
}

std::string Response::getResponse()
{
  return this->header + "\r\n" + this->data;
}

int Response::getResponseSize()
{
  return this->getResponse().size();
}

void Response::htmlResponse(std::string filepath)
{
  this->header = "HTTP/1.1 200 OK\r\n";
  this->header += "Content-Type: text/html\r\n";
  this->data = loadFile(filepath, false);
}

void Response::imageResponse(std::string filepath)
{
  auto imgType = split(filepath, ".")[1];
  std::string file = loadFile(filepath, true);
  this->header = "HTTP/1.1 200 OK\r\n";
  this->header += "Content-Type: image/" + imgType + "\r\n";
  this->header += "Content-Length: " + std::to_string(file.size()) + "\r\n";
  this->data = file;
}

void Response::notFound()
{
  htmlResponse("assets/oops.html");
  return;
}