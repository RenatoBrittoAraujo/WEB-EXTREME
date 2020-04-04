#ifndef GLOBALS
#define GLOBALS

#include <map>
#include <string>

class FileLoad
{
public:
  FileLoad(std::string filePath, bool rawFormat) :
    filePath(filePath), rawFormat(rawFormat)
  {
    isInit = true;
  }
  FileLoad() {}
  std::string filePath;
  bool isInit = false;
  bool rawFormat;
};

std::map<std::string, FileLoad> router = 
{
    { "/", FileLoad("assets/about.html", false) },
    { "/favicon.ico", FileLoad("assets/favicon.ico", true) }
};

#endif