#ifndef TABLE
#define TABLE

/*
  TABLE STRUCTURE:
  First 4 bytes of tablefile contains how many instances it has
  
  Each instance is divided into:
    ID | INSTANCE STATUS | NEXT INSTANCE BYTE RIGHT AFTER $ (or EOF) | field1 | field2 ... $
  Characters '|' and '$' are literally present in file to divide up
  
  Instances have Instance status which can have 3 values:
    - NONE: invalid
    - ACTIVE: valid instance
    - DELETED: invalid instance that should be skipped
*/

struct TableNotFoundException : public std::exception
{
  const char *what() const throw()
  {
    return "table was not found";
  }
};

struct TableAlreadyExistsException : public std::exception
{
  const char *what() const throw()
  {
    return "table exists already";
  }
};

enum InstStatus
{
  NONE = 0,
  ACTIVE,
  DELETED
};

class Table
{
public:
  Table() {}
  // Loads a table
  Table(std::string tablename);
  // Creates new table
  Table(std::string tablename, std::vector<std::string> fields);
  // Returns table name
  std::string getName();
  // Returns how many instances table has
  int count();
  // Queries a table element by id
  // Throws exception in case of failure
  std::map<std::string, std::string> getElement(int id);
  // Gets all instances of table
  std::vector<std::map<std::string, std::string>> getAllInstances();
  // Adds a new element to table 
  // Returns element ID
  int addElement(std::vector<std::map<std::string, std::string>> fields);
  // Removes element with said ID
  // Returns true if succeed and false if element not found
  bool removeElement(int id);
private:
  // Returns true if table exits
  static bool checkTableExists(std::string tablename);
  std::string tablename;
};

#endif