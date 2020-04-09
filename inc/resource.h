#ifndef RESOURCE
#define RESOURCE

#include "request.h"
#include "database.h"

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
  Resource(std::string table);
  ResourceRet handle(Request request);

  // Resource pages
  virtual ResourceRet index(Request request) { return ResourceRet(); }
  virtual ResourceRet createGET(Request request) { return ResourceRet(); }
  virtual ResourceRet createPOST(Request request) { return ResourceRet(); }
  virtual ResourceRet show(Request request, int index) { return ResourceRet(); }
  virtual ResourceRet destroy(Request request, int index) { return ResourceRet(); }
  virtual ResourceRet update(Request request, int index) { return ResourceRet(); }
  
  // Singleton resource page
  virtual ResourceRet page(Request request) { return ResourceRet(); }
private:
  static const std::map<std::string, std::string> asciiConverter;
protected:
  bool customURLHandling = false;
  bool singletonPage = false;

  Table table;
  
  // Returns instace in resource table
  std::map<std::string, std::string> getInstance(int index);
  // Returns all instances in resource table
  std::vector<std::map<std::string, std::string>> getAllInstances();
  // Parses a post request's data
  std::map<std::string, std::string> postParse(std::string raw_text);
  // Retuns all fields in resource table
  std::vector<std::string> getFields();

  std::vector<std::string> fields;
};

#endif