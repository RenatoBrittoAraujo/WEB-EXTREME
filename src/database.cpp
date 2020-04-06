#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <fstream>

#include "database.h"

Database::Database()
{
}

Table Database::createTable(std::string tablename, 
  std::vector<std::string> fields)
{
  return Table(tablename, fields);
}

bool Database::destroyTable(std::string tablename)
{
  std::fstream file("db/" + tablename);
  if (file.is_open())
  {
    file.close();
    return !remove(std::string("db/" + tablename).c_str());
  }
  return false;
}

std::vector<std::map<std::string, std::string>> 
  Database::getAllInstances(std::string tablename)
{
  if (not Database::tableExists(tablename))
  {
    throw TableNotFoundException();
  }
  return Table(tablename).getAllInstances();
}

std::vector<Table> Database::getTables()
{
  std::vector<Table> tables;
  DIR *dir;
  dirent *pdir;
  dir = opendir("db/");
  while ((pdir = readdir(dir)) != NULL)
  {
    if (pdir->d_name[0] == '.')
      continue;
    tables.push_back(Table(std::string(pdir->d_name)));
  }
  closedir(dir);
  free(pdir);
  return tables;
}

Table Database::getTable(std::string tablename)
{
  Table table;
  table = Table(tablename);
  return table;
}

bool Database::tableExists(std::string tablename)
{
  std::fstream file("db/" + tablename);
  if (file.is_open())
  {
    file.close();
    return true;
  }
  return false;
}