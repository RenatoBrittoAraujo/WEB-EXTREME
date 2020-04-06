#include <string>
#include <vector>

#include "request.h"

Request::Request(const std::string request)
{
  request_text = request;
}

REQUEST_TYPE Request::getType()
{
  if (this->type != REQUEST_TYPE::INVALID)
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