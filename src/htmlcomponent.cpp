#include "htmlcomponent.h"
#include "helpers.h"



HTMLComponent::HTMLComponent(std::string file)
{
  this->file = loadFile("assets/" + file, true);
}

bool HTMLComponent::addSubstitution(std::string element, 
  std::string to_change)
{
  std::string oldFile = this->file;
  int subst = 0;
  while (this->file = replace(this->file, element, 
    to_change), this->file != oldFile and subst < 100)
  {
    subst++;
    oldFile = this->file;
  }
  if (subst >= 100)
  {
    throw UncontrolledHTMLComponent();
  }
  return subst > 0;
}

std::string HTMLComponent::getFile()
{
  return this->file;
}
