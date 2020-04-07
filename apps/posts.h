#ifndef POSTS
#define POSTS

#include "resource.h"
#include "helpers.h"
#include "database.h"

class Posts : public Resource
{
public:
  Posts() {}
  ResourceRet index(Request request);
  ResourceRet show (Request request, int index);
};

ResourceRet Posts::index(Request request)
{
  std::cout<<"REACH INDEX"<<std::endl;
  std::string file = loadFile("assets/posts/index.html", false);
  std::cout<<file<<std::endl<<"AFTER:\n";
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
  std::cout<<file<<std::endl;
  ResourceRet ret;
  ret.response_status = RESPONSE_STATUS::OK;
  ret.data = file;
  return ret;
}

ResourceRet Posts::show(Request request, int index)
{
  std::cout<<"REACH SHOW WITH "<<index<<std::endl;
  std::string file = loadFile("assets/posts/show.html", false);
  std::string content;
  Table posts("post");
  std::map<std::string, std::string> instance;
  try
  {
    instance = posts.getElement(index);
  }
  catch(const std::exception& e)
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
  std::cout << file << std::endl;
  ResourceRet ret;
  ret.response_status = RESPONSE_STATUS::OK;
  ret.data = file;
  return ret;
}

// ResourceRet Posts::destroy(Request request)
// {
// }

#endif