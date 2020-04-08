#include "posts.h"
#include "helpers.h"
#include "database.h"

ResourceRet Posts::index(Request request)
{
  std::string file = loadFile("assets/posts/index.html", false);
  std::string content;
  Table posts("post");
  auto instances = posts.getAllInstances();
  auto fields = posts.getFields();
  std::string endline = "\r\n";
  content += "=======" + endline + "<br>";
  for (auto instance : instances)
  {
    for (auto field : fields)
    {
      content += field + ": " + instance[field] + "<br>";
    }
    content += "<br>=======" + endline + "<br>";
  }
  replace(file, "<content>", content);
  ResourceRet ret(RESPONSE_STATUS::OK);
  ret.data = file;
  return ret;
}

ResourceRet Posts::show(Request request, int index)
{
  std::string file = loadFile("assets/posts/show.html", false);
  std::string content;
  Table posts("post");
  std::map<std::string, std::string> instance;
  try
  {
    instance = posts.getElement(index);
  }
  catch (const std::exception &e)
  {
    return ResourceRet();
  }
  auto fields = posts.getFields();
  std::string endline = "\r\n";
  content += "=======" + endline + "<br>";
  for (auto field : fields)
  {
    content += field + ": " + instance[field] + endline;
  }
  replace(file, "<content>", content);
  ResourceRet ret(RESPONSE_STATUS::OK);
  ret.data = file;
  return ret;
}

ResourceRet Posts::createGET(Request request)
{
  ResourceRet ret(RESPONSE_STATUS::SEE_OTHER);
  ret.redirect_to = "https://www.google.com";
  return ret;
}

ResourceRet Posts::createPOST(Request request)
{
  return ResourceRet();
}

ResourceRet Posts::destroy(Request request, int index)
{
  return ResourceRet();
}