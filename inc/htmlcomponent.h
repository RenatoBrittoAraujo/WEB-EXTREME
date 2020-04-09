#ifndef HTML_COMPONENT
#define HTML_COMPONENT

#include <string>

struct UncontrolledHTMLComponent : public std::exception
{
  const char *what() const throw()
  {
    return "HTML Component is substituting infinitely";
  }
};

class HTMLComponent
{
public:
  HTMLComponent(std::string file);
  bool addSubstitution(std::string element, std::string file);
  std::string getFile();
  
private:
  std::string file;
};

#endif