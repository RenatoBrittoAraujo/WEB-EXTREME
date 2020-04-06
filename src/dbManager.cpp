#include <iostream>
#include <set>

#include "database.h"
#include "helpers.h"

struct Command
{
  Command(std::string commandName, std::string commandDescription) : 
    commandName(commandName), commandDescription(commandDescription)
  {
  }
  std::string commandName;
  std::string commandDescription;
};

std::vector<Command> commands =
    {
        Command("help", "Show available commands"),
        Command("exit", "Exits database management"),
        Command("create_table [table name] [field 1] [field 2] ...", "Creates a new table"),
        Command("destroy_table [table name]", "Destroys table"),
        Command("get_el [table name] [element ID]", "Returns fields of given element on given table"),
        Command("add_el [table name] [field 1]=\"[value1part1] [value2part2]\"  [field 2]=[value2] ...", "Creates a new element on a existent table"),
        Command("rem_el [table name] [element ID]", "Removes element from table"),
        Command("list_tables", "Lists all existent tables"),
        Command("list_elements [table name]", "Lists all elements in table (may be huge)"),
        Command("table_size [table name]", "Show number of elements in table"),
        Command("fields [table name]", "Show fields in table"),
};

bool test_table(std::string tablename)
{
  if (!Database::tableExists(tablename))
  {
    std::cout << "Table " + tablename + " was not found" << std::endl;
    return false;
  }
  return true;
}

void help()
{
  std::cout << "Commands available:" << std::endl;
  for (auto command : commands)
  {
    std::cout << "=> " << command.commandName << std::endl
              << command.commandDescription << std::endl
              << std::endl;
  }
}

void list_tables()
{
  auto list = Database::getTables();
  if (list.empty())
  {
    std::cout << "No tables were found" << std::endl;
  }
  for (auto table : list)
  {
    std::cout << "- " << table.getName() << std::endl;
  }
}

void table_size(std::string tablename)
{
  if (not test_table(tablename))
    return;
  std::cout << tablename + " has " << Table(tablename).count() 
    << " elements" << std::endl;
}

void create_table(std::string tablename, std::vector<std::string> args)
{
  if (Database::tableExists(tablename))
  {
    std::cout << "Table already exists" << std::endl;
    return;
  }
  args.erase(args.begin());
  args.erase(args.begin());
  Table(tablename, args);
  std::cout << "Table created" << std::endl;
}

void destroy_table(std::string tablename)
{
  if (not test_table(tablename))
    return;
  bool destroyed = !remove(("db/" + tablename).c_str());
  if (destroyed)
    std::cout << "Table was destroyed" << std::endl;
  else
    std::cout << "Table could not be removed" << std::endl;
}

void fields(std::string tablename)
{
  if (not test_table(tablename))
    return;
  auto fields = Table(tablename).getFields();
  for (auto field : fields)
    std::cout << field << std::endl;
}

void add_el(std::string tablename, std::vector<std::string> values)
{
  if (not test_table(tablename))
    return;
  values.erase(values.begin());
  values.erase(values.begin());
  std::map<std::string, std::string> elFields;
  bool onMultipleCall = false;
  std::string lastField = "";
  for (auto keyvalue : values)
  {
    if (onMultipleCall) 
    {
      if (keyvalue[keyvalue.size() - 1] == '"')
      {
        onMultipleCall = false;
        keyvalue.pop_back();
      }
      elFields[lastField] += " " + keyvalue;
      continue;
    }
    auto key = split(keyvalue, "=")[0];
    auto value = split(keyvalue, "=")[1];
    if (value[0] == '"') 
    {
      onMultipleCall = true;
      lastField = key;
      value = value.substr(1);
    }
    if (value[value.size() - 1] == '"')
    {
      onMultipleCall = false;
      value = value.substr(0, value.size() - 1);
    }
    elFields[key] = value;
  }
  if (onMultipleCall) 
  {
    std::cout << "Invalid parsing" << std::endl;
    return;
  }
  try
  {
    Table(tablename).addElement(elFields);
  }
  catch (InvalidFieldException& e)
  {
    std::cout << "Invalid fields, available field are:" << std::endl;
    fields(tablename);
    return;
  }
  std::cout << "Element created" << std::endl;
}

void get_el(std::string tablename, int id)
{
  if (not test_table(tablename))
    return;
  Table table(tablename);
  std::map<std::string, std::string> element;
  try
  {
    element = table.getElement(id);
  } 
  catch (InvalidQuery &e)
  {
    std::cout << "Query is invalid" << std::endl;
    return;
  }
  for (auto field : table.getFields())
  {
    std::cout << field << ": " << element[field] << std::endl;
  }
}

void list_elements(std::string tablename)
{
  if (not test_table(tablename))
    return;
  Table table(tablename);
  auto elements = table.getAllInstances();
  auto fields = table.getFields();
  for (auto element : elements)
  {
    std::cout << "id: " << element["id"] << std::endl;
    for (auto field : fields)
    {
      std::cout << field << ": " << element[field] << std::endl;
    }
  }
}

void rem_el(std::string tablename, int id)
{
  if (not test_table(tablename))
    return;
  Table table(tablename);
  bool ok = table.removeElement(id);
  std::cout << (ok ? "Element removed" : "Could not remove element") << std::endl;
}

/* == MACROS == */

#define CHECK(NAME, ARGS)                                            \
  else if (parsed_input[0] == NAME) if (parsed_input.size() != ARGS) \
  std::cout << "Invalid command" << std::endl; else

#define CHECKGT(NAME, ARGS)                                         \
  else if (parsed_input[0] == NAME) if (parsed_input.size() < ARGS) \
  std::cout << "Invalid command" << std::endl; else

    void
    Database::DBMode()
{
  std::cout << "Database management mode. Type \"help\" for available commands" 
    << std::endl << std::endl;
  while (true)
  {
    std::string input;
    std::cout << "COMMAND: ";
    std::getline(std::cin, input);
    if (input.empty())
      continue;
    std::cout << std::endl;
    auto parsed_input = split(input, " ");
    if (parsed_input[0] == "exit") exit(0);
    CHECK("help", 1) help();
    CHECK("list_tables", 1) list_tables();
    CHECK("table_size", 2) table_size(parsed_input[1]);
    CHECKGT("create_table", 3) create_table(parsed_input[1], parsed_input);
    CHECK("destroy_table", 2) destroy_table(parsed_input[1]);
    CHECK("fields", 2) fields(parsed_input[1]);
    CHECKGT("add_el", 3) add_el(parsed_input[1], parsed_input);
    CHECK("get_el", 3) get_el(parsed_input[1], std::stoi(parsed_input[2]));
    CHECK("list_elements", 2) list_elements(parsed_input[1]);
    CHECK("rem_el", 3) rem_el(parsed_input[1], std::stoi(parsed_input[2]));
    else std::cout << "Command not found" << std::endl;
    std::cout << std::endl;
  }
}