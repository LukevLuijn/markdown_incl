//
// Created by luke on 10-08-21.
//

#ifndef LIB_UTILS_COMMAND_H
#define LIB_UTILS_COMMAND_H

#include <cstring>
#include <vector>
#include <string>

namespace Utils
{
    class CommandlineArgument
    {
    public:
        /**
         * default constructor
         */
        CommandlineArgument();
        /**
         * default constructor
         *
         * @param anArgumentNumber  index of argument
         * @param aVariable         name of argument
         * @param aValue            value of argument
         */
        CommandlineArgument(unsigned long anArgumentNumber, std::string aVariable, std::string aValue);
        /**
         * assignment operator
         *
         * @param aCommandlineArgument  to be assigned variable
         * @return                      assigned variable
         */
        CommandlineArgument& operator=(const CommandlineArgument& aCommandlineArgument);
        /**
         * equal operator
         *
         * @param anArgumentNumber  to be compared to variable
         * @return                  true if both argument numbers are equal
         */
        bool operator==(uint64_t anArgumentNumber) const;
        /**
         * equal operator
         *
         * @param aVariable to be compared to variable
         * @return          true if both argument names are equal
         */
        bool operator==(const std::string& aVariable) const;
        /**
         * equal operator
         *
         * @param aCommandlineArgument  to be compared to CommandLineArgument
         * @return                      true if both CommandLineArguments are equal
         */
        bool operator==(const CommandlineArgument& aCommandlineArgument) const;
        /**
         * less-then operator
         *
         * @param aCommandlineArgument  to be compared to CommandLineArgument
         * @return                      true if index of 'this' is lower
         */
        bool operator<(const CommandlineArgument& aCommandlineArgument) const;

    public:
        unsigned long argumentNumber; /** index of argument*/
        std::string variable;         /** name of argument*/
        std::string value;            /** value of argument*/
    };

    class [[maybe_unused]] Command
    {
    public:
        /**
         * extract commandline arguments from given parameters
         *
         * @param argc argument count
         * @param argv argument value
         */
        [[maybe_unused]] static void setCommandLineArguments(int argc, char* argv[]);
        /**
         * check if argument with name: variable is given to program
         *
         * @param variable  name of argument
         * @return          true if argument is given
         */
        [[maybe_unused]] static bool isArgGiven(const std::string& variable);
        /**
         * get CommandLineArgument with name: variable
         *
         * @param   variable    name of requested CommandLineArgument
         * @return              CommandLineArgument with name: variable
         * @throws              std::invalid_argument if requested CommandLineArgument does not exist
         */
        [[maybe_unused]] static CommandlineArgument& getArg(const std::string& variable);
        /**
         * get CommandLineArgument with index: number
         *
         * @param number    index of requested CommandLineArgument
         * @return          CommandLineArgument with index: number
         * @throws          std::invalid_argument if requested CommandLineArgument does not exist
         */
        [[maybe_unused]] static CommandlineArgument& getArg(uint64_t number);
        /**
         * get CommandLineFiles
         *
         * @return the commandline files
         */
        [[maybe_unused]] static std::vector<std::string>& getCommandLineFiles();
        [[maybe_unused]] static void setCommandLineHelp(const std::vector<std::pair<std::string, std::string>>& options);
        [[maybe_unused]] static void help(const std::string& title);

    private:
        static std::vector<CommandlineArgument> commandlineArguments; /** list containing all CommandLineArguments*/
        static std::vector<std::string> commandlineFiles;             /** list containing all file entries*/
        static std::vector<std::pair<std::string, std::string>> options;
    };
}// namespace Utils

#endif// LIB_UTILS_COMMAND_H
