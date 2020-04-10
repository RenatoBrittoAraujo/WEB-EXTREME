#include <vector>
#include <fstream>
#include <map>
#include <set>

#include "response.h"
#include "request.h"
#include "helpers.h"
#include "resource.h"
#include "htmlcomponent.h"

/* ROUTING */

#include "posts.h"



/* END OF ROUTING */

/* HTML LINKS */

std::vector<std::string> htmlHeaders =
{
  // Page charset
  "<meta charset=\"URF-8\">",
  // Website title
  "<title> Web Extreme </title>"
  // Boostrap
   "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css\" integrity=\"sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh\" crossorigin=\"anonymous\">"
};

/* END OF HTML LINKS */

/* TEMPLATE SUBSTITUTIONS */

void Response::setTemplate(bool override_template)
{
  HTMLComponent completePage(
    override_template ? 
      "template/simpleTemplate.html" :
      "template/customTemplate.html");
  completePage.addSubstitution("<content>", this->data);
  completePage.addSubstitution("<header>", 
    HTMLComponent("template/header.html").getFile());
  this->data = completePage.getFile();
}

/* END OF TEMPLATE SUBSTITUTIONS */

const std::map<std::string, std::string>
    Response::fileToContentType =
{
  {"css", "text/css"},
  {"html", "text/html"},
  {"js", "application/javascript"},
  {"png", "image/png"},
  {"jpg", "image/jpg"},
  {"ico", "image/ico"}
};

const std::set<std::string> Response::imageExtensions =
{
  "png",
  "ico",
  "jpg"
};

Response::Response(Request req)
{
  this->findRequestResponse(req);

  if (this->isHTML(this->data))
  {
    this->setTemplate(false);
    for (auto header : htmlHeaders)
    {
      if (this->data.find(header) == this->data.npos)
        this->addHTMLHeader(header);
    }
  }

  this->buildHeaders();
}

void Response::findRequestResponse(Request req)
{
  std::map<const std::string, Route> router =
  {
    // Website icon
    {"/favicon.ico", Route("favicon.ico")},
    // Route to open posts index at home page
    {"/", Route(new Posts())},
    // Route to deal with show, delete, edit and create of posts 
    {"posts", Route(new Posts())},
    // About page
    {"/about", Route("about.html")}
  };
  
  std::string requestRoute = req.getPath();
  Route route;
  if (requestRoute.find("assets") != requestRoute.npos)
  {
    if (req.getType() == REQUEST_TYPE::GET)
    {
      this->raw_response = true;
      this->assetResponse(requestRoute);
    }
    else
      notFound();
  }
  else
  {
    if (not router.count(requestRoute))
    {
      auto resource = split(requestRoute, "/")[0];
      if (not router.count(resource))
      {
        notFound();
        return;
      }
      else
        route = router[resource];
    }
    else
    {
      route = router[requestRoute];
    }
    if (route.response_type == RESPONSE_TYPE::STATIC)
    {
      if (req.getType() == REQUEST_TYPE::GET)
        this->fileResponse(route.file);
      else
      {
        notFound();
      }
    }
    else
    {
      this->resourceReponse(route, req);
    }
  }
  if (this->header.empty() and this->data.empty())
  {
    notFound();
  }
}

std::string Response::getResponse()
{
  return this->header + "\r\n" + this->data;
}

int Response::getResponseSize()
{
  this->response_status = RESPONSE_STATUS::OK;
  return this->getResponse().size();
}
  
void Response::notFound()
{
  this->fileResponse("assets/oops.html");
}

void Response::redirectResponse(std::string redirect_to)
{
  this->response_status = RESPONSE_STATUS::SEE_OTHER;
  this->contentType = "html";
  this->data = "<script> window.location = \"" + redirect_to + "\"</script>\r\n";
}

void Response::resourceReponse(Route route, Request req)
{
  auto res = route.resource->handle(req);
  delete route.resource;
  if (res.response_status == RESPONSE_STATUS::NOT_FOUND)
  {
    notFound();
    return;
  }
  else if (not res.redirect_to.empty())
  {
    this->redirectResponse(res.redirect_to);
    return;
  }
  this->data = res.data;
  this->response_status = res.response_status;
  this->contentType = "html";
  if (not res.additional_headers.empty())
    this->responseHeaders.push_back(res.additional_headers);
}

void Response::assetResponse(std::string asset)
{
  // Security against accesing system's files
  if (asset.find("..") != asset.npos)
  {
    this->notFound();
    return;
  }
  std::string path;
  auto args = split(asset, "/");
  for (auto arg : args)
  {
    if (arg == "assets")
      path += arg;
    else if (not path.empty())
    {
      path += "/" + arg;
    }
  }
  this->fileResponse(path);
}

void Response::fileResponse(std::string file)
{
  if (file.find("assets") == file.npos)
    file = "assets/" + file;
  std::ifstream filep(file);
  if (not filep.is_open())
  {
    this->notFound();
    return;
  }
  filep.close();
  auto args = split(file, ".");
  std::string data = loadFile(file, true);
  this->response_status = RESPONSE_STATUS::OK;
  this->contentType = args[1];
  this->data = data + "\r\n";
}

void Response::buildHeaders()
{
  this->header = "HTTP/1.1 " +
                 std::to_string(this->response_status) + " " +
                 response_status_string.find(this->response_status)->second + "\r\n";
  this->header += "Content-Type: " +
                  this->fileToContentType.find(this->contentType)->second + "\r\n";
  this->header += "Content-Length: " + std::to_string(data.size()) + "\r\n";
  for (auto header : this->responseHeaders)
  {
    this->header += header + "\r\n";
  }
}

bool Response::isImage(std::string file)
{
  if (this->imageExtensions.count(split(file, ".")[1]))
    return true;
  return false;
}

void Response::addHTMLHeader(std::string header)
{
  this->data = replace(this->data, "</head>", std::string(header + "\r\n</head>"));
}

bool Response::isHTML(std::string data)
{
  return data.find("<html>") != data.npos or
         data.find("<head>") != data.npos or
         data.find("<body>") != data.npos or
         this->contentType == "html";
}