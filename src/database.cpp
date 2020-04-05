#include <iostream>
#include <string>
#include <vector>

#include "database.h"


/* ====== DATABASE MANAGEMENT IO ====== */

struct Command
{
  Command(std::string commandName, std::string commandDescription) :
    commandName(commandName), commandDescription(commandDescription)
  {}
  std::string commandName;
  std::string commandDescription;
};

std::vector<Command> commands =
{
  Command("create", "dick balls ballks dick"),
  Command("destroy", ""),
  Command("update", ""),
  Command("check_attr", ""),
  Command("list", ""),
  Command("exit", "Exits the program")
};

void helpio()
{

  std::cout << "Commands available:" << std::endl;
  for (auto command : commands)
  {
    std::cout << "=> " << command.commandName << std::endl
              << command.commandDescription << std::endl
              << std::endl;
  }
}

void Database::DBMode()
{
  std::cout << "Database management mode. Type \"help\" for available commands" << std::endl;
  while (true)
  {
    std::string input;
    std::cout << "COMMAND: ";
    std::getline(std::cin, input);
    if (input.find("help") != input.npos)
    {
      helpio();
    }
  }
}