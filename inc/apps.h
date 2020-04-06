#ifndef APPS
#define APPS

#include <map>

#include "posts.h"

std::map<std::string, Resource> apphandler =
{
  { "posts", Posts() }
};

Resource getApp(std::string appname)
{
  return apphandler[appname];
}

#endif