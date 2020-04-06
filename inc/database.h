#ifndef DATABASE
#define DATABASE

#include <map>
#include <vector>
#include <string>
#include <exception>

#include "table.h"

class Database
{
public:
  // Creates a database or mounts current one
  Database();
  // IO dialog for dealing with database
  static void DBMode();
  // Creates a table with given name and fields (table names are unique)
  // Returns true if succeed and false if fail
  static Table createTable(std::string tablename, std::vector<std::string> fields);
  // Destroys a table with giver name
  // Returns true if succeed and false if fail
  static bool destroyTable(std::string tablename);
  // Returns a list of all instances in a table
  // Throws exception in case of failure to find table
  static std::vector<std::map<std::string, std::string>> 
    getAllInstances(std::string tablename);
  // Returns a list of all tables
  static std::vector<Table> getTables();
  // Returns table with given name
  static Table getTable(std::string tablename);
  // Returns true if table exists and false otherwise
  static bool tableExists(std::string tablename);

private:

};

#endif