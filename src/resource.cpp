#include "resource.h"
#include "helpers.h"
#include "database.h"

Resource::Resource(std::string table)
{
  this->table = Table(table); 
}

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

std::map<std::string, std::string> 
  Resource::postParse(std::string raw_text)
{
  auto args = split(raw_text, "&");
  std::map<std::string, std::string> ret;
  for (auto arg : args)
  {
    std::string key;
    int i = 0;
    while (arg[i] != '=')
      key += arg[i++];
    std::string parsedValue = arg.substr(i + 1);
    for (auto &c : parsedValue)
      if (c == '+')
        c = ' ';
    while (true)
    {
      auto pos = parsedValue.find("%");
      if (pos == parsedValue.npos)
        break;
      std::string in = parsedValue.substr(pos + 1, 2);
      std::string out = this->asciiConverter.find(in)->second;
      parsedValue = replace(parsedValue, "%" + in, out);
    }
    ret[key] = parsedValue;
  }
  return ret;
}

std::map<std::string, std::string> Resource::getInstance(int index)
{
  std::map<std::string, std::string> instance;
  try
  {
    instance = table.getElement(index);
  }
  catch (const std::exception &e)
  {
  }
  return instance;
}

std::vector<std::map<std::string, std::string>> Resource::getAllInstances()
{
  return this->table.getAllInstances();
}

std::vector<std::string> Resource::getFields()
{
  return this->table.getFields();
}

const std::map<std::string, std::string> 
  Resource::asciiConverter =
{
  {"20", "-"},
  {"21", "!"},
  {"22", "\""},
  {"23", "#"},
  {"24", "$"},
  {"25", "%"},
  {"26", "&"},
  {"27", "'"},
  {"28", "("},
  {"29", ")"},
  {"2A", "*"},
  {"2B", "+"},
  {"2C", ","},
  {"2D", "-"},
  {"2E", "."},
  {"2F", "/"},
  {"30", "0"},
  {"31", "1"},
  {"32", "2"},
  {"33", "3"},
  {"34", "4"},
  {"35", "5"},
  {"36", "6"},
  {"37", "7"},
  {"38", "8"},
  {"39", "9"},
  {"3A", ":"},
  {"3B", ";"},
  {"3C", "<"},
  {"3D", "="},
  {"3E", ">"},
  {"3F", "?"},
  {"40", "@"},
  {"41", "A"},
  {"42", "B"},
  {"43", "C"},
  {"44", "D"},
  {"45", "E"},
  {"46", "F"},
  {"47", "G"},
  {"48", "H"},
  {"49", "I"},
  {"4A", "J"},
  {"4B", "K"},
  {"4C", "L"},
  {"4D", "M"},
  {"4E", "N"},
  {"4F", "O"},
  {"50", "P"},
  {"51", "Q"},
  {"52", "R"},
  {"53", "S"},
  {"54", "T"},
  {"55", "U"},
  {"56", "V"},
  {"57", "W"},
  {"58", "X"},
  {"59", "Y"},
  {"5A", "Z"},
  {"5B", "["},
  {"5C", "\\"},
  {"5D", "]"},
  {"5E", "^"},
  {"5F", "_"},
  {"60", "`"},
  {"61", "a"},
  {"62", "b"},
  {"63", "c"},
  {"64", "d"},
  {"65", "e"},
  {"66", "f"},
  {"67", "g"},
  {"68", "h"},
  {"69", "i"},
  {"6A", "j"},
  {"6B", "k"},
  {"6C", "l"},
  {"6D", "m"},
  {"6E", "n"},
  {"6F", "o"},
  {"70", "p"},
  {"71", "q"},
  {"72", "r"},
  {"73", "s"},
  {"74", "t"},
  {"75", "u"},
  {"76", "v"},
  {"77", "w"},
  {"78", "x"},
  {"79", "y"},
  {"7A", "z"},
  {"7B", "{"},
  {"7C", "|"},
  {"7D", "}"},
  {"7E", "~"},
  {"7F", "DEL"},
};