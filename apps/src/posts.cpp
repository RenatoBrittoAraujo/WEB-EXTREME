#include "posts.h"
#include "helpers.h"
#include "database.h"
#include "htmlcomponent.h"

ResourceRet Posts::index(Request request)
{
  Table posts("post");
  auto instances = posts.getAllInstances();
  auto fields = posts.getFields();
  HTMLComponent index("posts/index.html");
  std::string content;
  fields.push_back("id");
  for (auto instance : instances)
  {
    HTMLComponent post("posts/post.html");
    for (auto field : fields)
    {
      post.addSubstitution("<post." + field + ">", instance[field]);    
    }
    content += post.getFile();
  }
  index.addSubstitution("<posts>", content);
  ResourceRet ret(RESPONSE_STATUS::OK);
  ret.data = index.getFile();
  return ret;
}

ResourceRet Posts::show(Request request, int index)
{
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
  HTMLComponent post("posts/post.html");
  for (auto field : fields)
  {
    post.addSubstitution("<post." + field + ">", instance[field]);
  }
  HTMLComponent file("posts/show.html");
  file.addSubstitution("<post>", post.getFile());
  ResourceRet ret(RESPONSE_STATUS::OK);
  ret.data = file.getFile();
  return ret;
}

ResourceRet Posts::createGET(Request request)
{
  ResourceRet ret(RESPONSE_STATUS::OK);
  ret.data = loadFile("assets/posts/new.html", true);
  return ret;
}

ResourceRet Posts::createPOST(Request request)
{
  Table table("post");
  auto args = split(request.getData(), "&");
  std::map<std::string, std::string> element;
  for (auto arg : args)
  {
    std::string key;
    int i = 0;
    while (arg[i] != '=') key += arg[i++];
    element[key] = arg.substr(i + 1);
  }
  for (auto v : element)
  {
    std::cout<<"("<<v.first<<"): ("<<v.second<<")"<<std::endl;
  }
  bool success = true;
  try
  {
    table.addElement(element);
  }
  catch(const std::exception& e)
  {
    std::cout<<e.what()<<std::endl;
    success = false;
  }
  ResourceRet ret;
  if (success)
  {
    ret.response_status = RESPONSE_STATUS::CREATED;
    ret.redirect_to = "/posts/" + std::to_string(table.count());
  }
  else
  {
    ret.response_status = RESPONSE_STATUS::OK;
    ret.redirect_to = "/posts/new";
  }
  return ret;
}

ResourceRet Posts::destroy(Request request, int index)
{
  Table table("post");
  try
  {
    table.removeElement(index);
  }
  catch(const std::exception& e)
  {
  }
  ResourceRet ret(RESPONSE_STATUS::OK);
  ret.redirect_to = "/posts";
  return ret;
}