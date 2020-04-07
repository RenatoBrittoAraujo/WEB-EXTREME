#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>

#include "helpers.h"

std::vector<std::string> split(std::string str, std::string sep)
{
  char *cstr = const_cast<char *>(str.c_str());
  char *current;
  std::vector<std::string> arr;
  current = strtok(cstr, sep.c_str());
  while (current != NULL)
  {
    arr.push_back(current);
    current = strtok(NULL, sep.c_str());
  }
  return arr;
}

bool replace(std::string &str, const std::string &from, const std::string &to)
{
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

std::string loadFile(const std::string path, bool raw)
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