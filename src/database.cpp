#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <fstream>

#include "database.h"
#include "helpers.h"
#include "table.h"

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
  if (not Table::checkTableExists(tablename))
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

void Database::runMigrations()
{
  std::string raw_migrations = loadFile("migrations", false);
  auto migrations = split(raw_migrations, "$");
  for (auto raw_migration : migrations)
  {
    std::string migration = raw_migration;
    migration.pop_back();
    migration.pop_back();
    auto args = split(migration, " ");
    if (migration.size() < 3 or
        migration.empty() or
        args.size() < 2)
      continue;
    if (not Table::checkTableExists(args[0]))
    {
      std::string tableName = args[0];
      args.erase(args.begin());
      Database::createTable(tableName, args);
    }
  }
}