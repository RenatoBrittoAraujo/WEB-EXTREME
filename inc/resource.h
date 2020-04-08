#ifndef RESOURCE
#define RESOURCE

#include "request.h"

struct ResourceRet
{
  ResourceRet()
  {
    response_status = RESPONSE_STATUS::NOT_FOUND;
  }
  ResourceRet(RESPONSE_STATUS res)
  {
    response_status = res;
  }
  RESPONSE_STATUS response_status;
  std::string data = "";
  std::string additional_headers = "";
  std::string redirect_to = "";
};

class Resource
{
public:
  Resource() {}
  ResourceRet handle(Request request);
  // Resource pages
  virtual ResourceRet index(Request request) { return ResourceRet(); }
  virtual ResourceRet createGET(Request request) { return ResourceRet(); }
  virtual ResourceRet createPOST(Request request) { return ResourceRet(); }
  virtual ResourceRet show(Request request, int index) { return ResourceRet(); }
  virtual ResourceRet destroy(Request request, int index) { return ResourceRet(); }
  virtual ResourceRet update(Request request, int index) { return ResourceRet(); }
  // Singleton page
  virtual ResourceRet page(Request request) { return ResourceRet(); }
private:
protected:
  bool customURLHandling = false;
  bool singletonPage = false;
  

};

#endif