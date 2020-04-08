#include "resource.h"
#include "helpers.h"

ResourceRet Resource::handle(Request request)
{
  auto args = split(request.getPath(), "/");
  ResourceRet ret;
  if (args.size() < 2 and this->singletonPage)
  {
    ret = page(request);
  }
  else if (args.size() < 2 and 
           request.getType() == REQUEST_TYPE::GET)
  {
    ret = index(request);
  }
  else
  {
    bool validNumber = true;
    try
    {
      std::stoi(args[1]);
    }
    catch (const std::exception &e)
    {
      validNumber = false;
    }
    if (args[1] == "new")
    {
      if (request.getType() == REQUEST_TYPE::GET)
        ret = createGET(request);
      else if (request.getType() == REQUEST_TYPE::POST)
        ret = createPOST(request);
    }
    else if (validNumber and
             request.getType() == REQUEST_TYPE::GET)
    {
      int index = std::stoi(args[1]);
      if (args.size() == 2)
        ret = show(request, index);
      else if (args.size() == 3 and args[2] == "delete")
        ret = destroy(request, index);
      else if (args.size() == 3 and args[2] == "edit")
        ret = update(request, index);
    }
  }
  return ret;
}