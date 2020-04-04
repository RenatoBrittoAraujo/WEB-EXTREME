#ifndef PAGE_GEN
#define PAGE_GEN

#include <string>

class PageGen
{
public:
  std::string getIndex();
  std::string getResourcePage(std::string resource_type, int resource_id);
  std::string getResourcePost();
private:

};

#endif