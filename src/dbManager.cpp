#include <iostream>

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
  Command("create_table [table name] [table field 1] [table field 2] ...", "Creates a new table"),
  Command("destroy_table [table name]", "Destroys table"),
  Command("update_el [table name] [element ID] [fields]", "Updates all fields of element in table"),
  Command("get_el [table name] [element ID]", "Returns fields of given element on given table"),
  Command("add_el [table name] [fields]", "Creates a new element on a existent table"),
  Command("list_tables", "Lists all existent tables"),
  Command("list_elements [table name]", "Lists all elements in table (may be huge)"),
  Command("peek_table [table name]", "Lists some elements in table"),
  Command("table_size [table name]", "Show number of elements in table")
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

/* == MACROS == */
#define CHECK(NAME, ARGS)                                            \
  else if (parsed_input[0] == NAME) if (parsed_input.size() != ARGS) \
  std::cout << "Invalid command" << std::endl; else

#define CHECKGT(NAME, ARGS)                                         \
  else if (parsed_input[0] == NAME) if (parsed_input.size() < ARGS) \
  std::cout << "Invalid command" << std::endl; else

void Database::DBMode()
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
    else std::cout << "Command not found" << std::endl;
    std::cout << std::endl;
  }
}