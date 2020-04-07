#ifndef RESOURCE
#define RESOURCE

#include "request.h"

struct ResourceRet
{
  ResourceRet()
  {
    response_status = RESPONSE_STATUS::NOT_FOUND;
  }
  RESPONSE_STATUS response_status;
  std::string data;
  std::string additional_headers;
};

class Resource
{
public:
  Resource() {}
  ResourceRet handle(Request request)
  {
    std::cout << "REACH RESOURCE HANDLE" << std::endl;
    std::cout<<"REQUEST PATH: "<<request.getPath()<<std::endl;
    auto args = split(request.getPath(), "/");
    for (auto arg : args)
      std::cout<<arg<<std::endl;
    if (args.size() == 1 and request.getType() == REQUEST_TYPE::GET)
    {
      std::cout<<"RET INDEX"<<std::endl;
      return index(request);
    }
    else if (args.size() == 2)
    {
      bool validNumber = true;
      try
      {
        std::stoi(args[1]);
      }
      catch(const std::exception& e)
      {
        validNumber = false;
      }
      if (args[1] == "new")
      {
        if (request.getType() == REQUEST_TYPE::GET)
          return createGET(request);
        else if (request.getType() == REQUEST_TYPE::POST)
          return createPOST(request);
        else return ResourceRet();
      }
      else if (validNumber)
      {
        int index = std::stoi(args[1]);
        if (request.getType() == REQUEST_TYPE::GET)
          return show(request, index);
        else if (request.getType() == REQUEST_TYPE::DELETE)
          return destroy(request, index);
        else if (request.getType() == REQUEST_TYPE::PUT)
          return update(request, index);
        else
          return ResourceRet();
      }
      else return ResourceRet();
    }
    else
      return ResourceRet();
  }
 
  virtual ResourceRet index(Request request) { return ResourceRet(); }
  virtual ResourceRet createGET(Request request) { return ResourceRet(); }
  virtual ResourceRet createPOST(Request request) { return ResourceRet(); }
  virtual ResourceRet show(Request request, int index) { return ResourceRet(); }
  virtual ResourceRet destroy(Request request, int index) { return ResourceRet(); }
  virtual ResourceRet update(Request request, int index) { return ResourceRet(); }

private:
};

#endif