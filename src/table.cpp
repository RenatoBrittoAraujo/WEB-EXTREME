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
  table.read((char *)&count, sizeof(int));
  table.close();
  return count;
}

std::map<std::string, std::string> Table::getElement(int id)
{
  std::map<std::string, std::string> element;
  if (count() < id)
  {
    throw InvalidQuery();
    return element;
  }
  auto table = getElPosition(id);
  while (table.get() != '|')
    ;
  auto status = getStatus(table);
  if (status != InstStatus::ACTIVE)
  {
    throw InvalidQuery();
    return element;
  }
  while (table.get() != '|')
    ;
  std::string buffer;
  char ch;
  while ((ch = table.get()) != '$')
    buffer += ch;
  table.close();
  auto orderedFields = split(buffer, "|");
  int indx = 0;
  for (auto field : this->getFields())
  {
    element[field] = orderedFields[indx++];
  }
  element["id"] = std::to_string(id);
  return element;
}

std::vector<std::map<std::string, std::string>>
Table::getAllInstances()
{
  std::vector<std::map<std::string, std::string>> instances;
  int numOfElements = count();
  for (int i = 1; i <= numOfElements; i++)
  {
    std::map<std::string, std::string> element;
    try
    {
      element = getElement(i);
    }
    catch (const InvalidQuery &e)
    {
      continue;
    }
    instances.push_back(element);
  }
  return instances;
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
  std::fstream table("db/" + this->tablename, 
    std::ios::binary | std::ios::in | std::ios::out);
  int newCount = count() + 1;
  table.write((char *)&newCount, sizeof(int));
  table.close();
  table.open("db/" + this->tablename, std::ios::app);

  // First 4 bytes: ID
  table.write((char *)(&newCount), sizeof(newCount));
  table << '|';
  // Next 2 bytes: Status
  int16_t status = InstStatus::ACTIVE;
  table.write((char *)(&status), sizeof(status));
  table << '|';
  // Content by field in order of input
  auto fields = this->getFields();
  for (size_t i = 0; i < fields.size(); i++)
  {
    table << instance[fields[i]];
    if (i != fields.size() - 1)
      table << '|';
  }
  table << '$';

  table.close();
}

bool Table::removeElement(int id)
{
  if (count() < id)
  {
    return false;
  }
  auto table = getElPosition(id);
  while (table.get() != '|')
    ;
  auto status = getStatus(table);
  if (status != InstStatus::ACTIVE)
  {
    return false;
  }
  int pos = table.tellg();
  table.close();
  std::fstream ctable("db/" + tablename, std::ios::binary 
    | std::ios::in | std::ios::out);
  ctable.seekg(pos - 2);
  status = InstStatus::DELETED;
  ctable.write((char *)(&status), sizeof(status));
  ctable.close();
}

int Table::getNumberOfFields()
{
  return this->getFields().size();
}

std::vector<std::string> Table::getFields()
{
  std::ifstream table("db/" + tablename);
  std::string header;
  for (int i = 0; i < 5; i++)
    table.get();
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

int Table::getID(std::ifstream &table)
{
  int ret;
  table.read((char *)&ret, sizeof(ret));
  return ret;
}

int16_t Table::getStatus(std::ifstream &table)
{
  int16_t ret;
  table.read((char *)&ret, sizeof(ret));
  return ret;
}

std::ifstream Table::getElPosition(int id)
{
  std::ifstream table("db/" + this->tablename, std::ios::binary);
  while (table.get() != '$')
    ;
  while (true)
  {
    int cid = getID(table);
    if (cid < id)
    {
      while (table.get() != '$')
        ;
    }
    else if (cid > id)
    {
      throw InvalidQuery();
      return std::ifstream();
    }
    else
    {
      break;
    }
  }
  return table;
}