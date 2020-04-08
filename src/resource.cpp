#include "resource.h"
#include "helpers.h"

ResourceRet Resource::handle(Request request)
{
  auto args = split(request.getPath(), "/");
  if (args.size() < 2 and request.getType() == REQUEST_TYPE::GET)
  {
    return index(request);
  }
  else if (args.size() == 2)
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
        return createGET(request);
      else if (request.getType() == REQUEST_TYPE::POST)
        return createPOST(request);
      else
        return ResourceRet();
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
    else
      return ResourceRet();
  }
  else
    return ResourceRet();
}