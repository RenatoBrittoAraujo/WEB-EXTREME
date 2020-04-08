#ifndef POSTS
#define POSTS

#include "resource.h"

class Posts : public Resource
{
public:
  Posts() {}
  ResourceRet index(Request request);
  ResourceRet show (Request request, int index);
  ResourceRet createGET(Request request);
  ResourceRet createPOST(Request request);
  ResourceRet destroy(Request request, int index);
};

#endif