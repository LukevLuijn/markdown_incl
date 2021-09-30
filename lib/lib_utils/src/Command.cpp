//
// Created by luke on 22-08-21.
//

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "Command.h"
#include "Console.h"

namespace Utils
{
    /* static */ std::vector<CommandlineArgument> Command::commandlineArguments;
    /* static */ std::vector<std::string> Command::commandlineFiles;
    /* static */ std::vector<std::pair<std::string, std::string>> Command::options;

    CommandlineArgument::CommandlineArgument()
        : argumentNumber(std::numeric_limits<unsigned long>::max()), variable(), value()
    {
    }
    CommandlineArgument::CommandlineArgument(unsigned long anArgumentNumber, std::string aVariable, std::string aValue)
        : argumentNumber(anArgumentNumber), variable(std::move(aVariable)), value(std::move(aValue))
    {
    }

    CommandlineArgument& CommandlineArgument::operator=(const CommandlineArgument& aCommandlineArgument)
    {
        if (this != &aCommandlineArgument)
        {
            argumentNumber = aCommandlineArgument.argumentNumber;
            variable = aCommandlineArgument.variable;
            value = aCommandlineArgument.value;
        }
        return *this;
    }
    bool CommandlineArgument::operator==(const unsigned long anArgumentNumber) const
    {
        return argumentNumber == anArgumentNumber;
    }
    bool CommandlineArgument::operator==(const std::string& aVariable) const
    {
        return variable == aVariable;
    }
    bool CommandlineArgument::operator==(const CommandlineArgument& aCommandlineArgument) const
    {
        return (argumentNumber == aCommandlineArgument.argumentNumber) && (variable == aCommandlineArgument.variable) &&
               (value == aCommandlineArgument.value);
    }
    bool CommandlineArgument::operator<(const CommandlineArgument& aCommandlineArgument) const
    {
        return argumentNumber < aCommandlineArgument.argumentNumber;
    }
    /* static */ void Command::setCommandLineArguments(int argc, char* argv[])
    {
        commandlineArguments.emplace_back(0, "Executable", argv[0]);

        for (int i = 1; i < argc; ++i)
        {
            char* currentArg = argv[i];
            size_t argLength = std::strlen(currentArg);

            if (currentArg[0] == '-')
            {
                bool inserted = false;
                for (size_t j = 0; j < argLength; ++j)
                {
                    if (currentArg[j] == '=')
                    {
                        std::string variable(currentArg, j);
                        std::string value(&currentArg[j + 1]);
                        commandlineArguments.emplace_back(i, variable, value);
                        inserted = true;
                    }
                }
                if (!inserted)
                {
                    std::string variable(currentArg);
                    std::string value("true");
                    commandlineArguments.emplace_back(i, variable, value);
                }
            }
            else
            {
                commandlineFiles.emplace_back(currentArg);
            }
        }
    }
    /* static */ bool Command::isArgGiven(const std::string& variable)
    {
        auto i = std::find(commandlineArguments.begin(),
                           commandlineArguments.end(),
                           static_cast<std::string>("-" + variable));
        return i != commandlineArguments.end();
    }
    /* static */ CommandlineArgument& Command::getArg(const std::string& variable)
    {
        auto i = std::find(commandlineArguments.begin(),
                           commandlineArguments.end(),
                           static_cast<std::string>("-" + variable));
        if (i == commandlineArguments.end())
        {
            throw std::invalid_argument("No such command line argument");
        }
        return *i;
    }
    /* static */ CommandlineArgument& Command::getArg(const uint64_t number)
    {
        if (number >= commandlineArguments.size())
        {
            throw std::invalid_argument("No such command line argument");
        }
        return commandlineArguments[number];
    }

    /* static */ std::vector<std::string>& Command::getCommandLineFiles()
    {
        return commandlineFiles;
    }
    /* static */ void Command::help(const std::string& title)
    {
        std::vector<std::string> lines(1, "");
        lines[0] = Console::convertToColor("[USAGE]", Console::BRIGHT_MAGENTA).append("\t").append(title);

        for(const auto& option : options)
        {
            // add option to header
            if (option == *options.begin())
                lines[0].append("\t|");
            lines[0].append(Console::convertToColor(option.first, Console::BRIGHT_CYAN)).append("|");

            // create entry of option
            std::string entry = "-";
            entry.append(Console::convertToColor(option.first, Console::BRIGHT_CYAN));
            entry.append("\t").append(option.second);
            lines.emplace_back(entry);
        }
        // print actual help
        for(const std::string& line : lines)
            std::cout << line << std::endl;
    }
    /* static */ void Command::setCommandLineHelp(const std::vector<std::pair<std::string, std::string>>& new_options)
    {
        options = new_options;
    }
}// namespace Utils
