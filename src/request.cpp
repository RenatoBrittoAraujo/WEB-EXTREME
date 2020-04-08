#include <string>
#include <vector>
#include <map>

#include "request.h"
#include "helpers.h"

Request::Request(const std::string request)
{
  request_text = request;
  auto splt = split(request_text, "\n");
  bool endOfHeaders = false;
  splt.erase(splt.begin());
  for (auto str : splt)
  {
    if (not endOfHeaders and str.find(": ") != str.npos)
    {
      this->headers.push_back(str);
      auto keyvalue = split(str, ": ");
      this->parsedHeaders[keyvalue[0]] = keyvalue[1];
    }
    else
    {
      if (endOfHeaders == false)
      {
        endOfHeaders = true;
        continue;
      }
      data += str;
    }
  }
  request_text = request;
}

REQUEST_TYPE Request::getType()
{
  if (this->type != REQUEST_TYPE::ANY)
  {
    return this->type;
  }
  std::string typeText;
  int i = 0;
  while (request_text[i] != ' ')
  {
    typeText += request_text[i++];
  }
  if (typeText == "GET")
    this->type = REQUEST_TYPE::GET;
  if (typeText == "POST")
    this->type = REQUEST_TYPE::POST;
  if (typeText == "PUT")
    this->type = REQUEST_TYPE::PUT;
  if (typeText == "DELETE")
    this->type = REQUEST_TYPE::DELETE;

  return this->type;
}

std::string Request::getPath()
{
  if (not this->path.empty())
  {
    return this->path;
  }
  std::vector<int> spcLoc;
  for (int i = 0; i < request_text.size(); i++)
  {
    if (request_text[i] == ' ')
    {
      spcLoc.push_back(i);
    }
    if (spcLoc.size() == 2)
    {
      break;
    }
  }
  int rem = 1 + (request_text[spcLoc[1] - 1] == '/' and 
    spcLoc[1] - 2 != spcLoc[0]);
  return this->path = request_text.substr(
    spcLoc[0] + 1, spcLoc[1] - spcLoc[0] - rem);
}

bool Request::isFaviconReq()
{
  return  request_text.find("GET /favicon.ico") 
          != request_text.npos;
}

std::vector<std::string> Request::getHeaders()
{
  return this->headers;
}

std::string Request::getData()
{
  return this->data;
}

std::string Request::getHeaderValue(std::string key)
{
  return this->parsedHeaders[key];
}

bool Request::headerHasKey(std::string key)
{
  return this->parsedHeaders[key] != "";
}