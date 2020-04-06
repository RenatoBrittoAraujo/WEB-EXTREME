#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "table.h"

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
  std::vector<std::map<std::string, std::string>> fields)
{

}

bool Table::removeElement(int id)
{

}

bool Table::checkTableExists(std::string tablename)
{
  std::ifstream table("db/" + tablename);
  bool ret = table.is_open();
  table.close();
  return ret;
}
