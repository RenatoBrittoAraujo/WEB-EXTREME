#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "response.h"
#include "request.h"

Response::Response(Request req)
{
  // ASSUMING VALID REQUEST
  response_text += "HTTP/1.1 200 OK\r\n";
  std::string file = router[req.getPath()];
  if (req.isFaviconReq())
  {
    response_text += "Content-Type: image/ico\r\n";
    response_text += "Content-Length : " + std::to_string(file.size());
    response_text += "\r\n\r\n";
    response_text += file;
  }
  else
  {
    response_text += "Content-Type: text/html\r\n\r\n";
    response_text += file;
  }
}

std::string Response::getResponse()
{
  return this->response_text;
}

int Response::getResponseSize()
{
  return this->response_text.size();
}

std::string Response::loadFile(const std::string path, bool raw)
{
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open())
  {
    return "";
  }
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> buffer(size);
  std::string file_contents;
  if (file.read(buffer.data(), size))
  {
    if (raw)
    {
      file_contents = std::string(buffer.begin(), buffer.end());
    } 
    else 
    {
      for (auto ch : buffer)
        if (ch == '\n')
          file_contents += "\r\n";
        else
          file_contents += ch;
    }
    file_contents += "\r\n";
  }
  return file_contents;
}