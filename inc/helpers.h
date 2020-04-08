#ifndef HELPERS
#define HELPERS

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> 
  split(std::string str, std::string sep);

std::string replace(std::string &str, const std::string &from, const std::string &to);

std::string loadFile(const std::string path, bool raw);

#endif