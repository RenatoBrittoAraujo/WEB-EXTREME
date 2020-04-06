#ifndef POSTS
#define POSTS

#include "resource.h"

class Posts : public Resource
{
public:
  Posts() {}
  std::string index(Request request) { return ""; }
  std::string show(Request request) { return ""; }
  std::string destroy(Request request) { return ""; }
};

#endif