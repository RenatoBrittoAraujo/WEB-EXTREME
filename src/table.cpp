#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <set>

#include "table.h"
#include "helpers.h"

Table::Table(std::string tablename)
{
  if (not Table::checkTableExists(tablename))
  {
    throw TableNotFoundException();
  }
  this->tablename = tablename;
}

Table::Table(std::string tablename, 
  std::vector<std::string> fields)
{
  if (Table::checkTableExists(tablename))
  {
    throw TableAlreadyExistsException();
  }
  this->tablename = tablename;
  std::ofstream table("db/" + tablename, std::ios::binary);
  int size = 0;
  table.write((char *)(&size), sizeof(size));
  table << '|';
  for (size_t i = 0; i < fields.size(); i++)
  {
    table << fields[i];
    if (fields.size() - 1 > i) 
      table << '|';
  }
  table << '$';
  table.close();
}

std::string Table::getName()
{
  return this->tablename;
}

int Table::count()
{
  std::ifstream table("db/" + tablename, std::ios::binary);
  int count;
  table.read((char *) &count, sizeof (int));
  table.close();
  return count;
}

std::map<std::string, std::string> Table::getElement(int id)
{

}

std::vector<std::map<std::string, std::string>> 
  Table::getAllInstances()
{

}

int Table::addElement(
  std::map<std::string, std::string> instance)
{
  std::set<std::string> remainingFields;
  for (auto field : this->getFields())
    remainingFields.insert(field);
  for (auto keyvalue : instance)
  {
    auto field = keyvalue.first;
    if (!remainingFields.count(field)) 
    {
      throw InvalidFieldException();
      return -1;
    }
    remainingFields.erase(field);
  }
  if (remainingFields.size() != 0)
  {
    throw InvalidFieldException();
    return -1;
  }
  std::fstream table("db/" + this->tablename, std::ios::binary | std::ios::in | std::ios::out);
  int newCount = count() + 1;
  table.write((char *) &newCount, sizeof(int));
  table.close();
  table.open("db/" + this->tablename, std::ios::app);
  table << "fuck";
  table.close();
}

bool Table::removeElement(int id)
{

}

int Table::getNumberOfFields()
{
  return this->getFields().size();
}

std::vector<std::string> Table::getFields()
{
  std::ifstream table("db/" + tablename);
  std::string header;
  for (int i=0;i<5;i++)table.get();
  char c;
  while (c = table.get(), c != '$')
    header += c;
  table.close();
  auto fields = split(header, "|");
  return fields;
}

bool Table::checkTableExists(std::string tablename)
{
  std::ifstream table("db/" + tablename);
  bool ret = table.is_open();
  table.close();
  return ret;
}