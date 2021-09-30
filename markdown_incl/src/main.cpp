
#include <iostream>
#include <string>

#include "Command.h"
#include "Console.h"
#include "FileIO.h"
#include "Misc.h"

void check_input(int argc, char** argv)
{
    auto print_help = []() {
        std::string program_title = "markdown_incl";
        Utils::Command::help(program_title);
        exit(EXIT_SUCCESS);
    };

    std::vector<std::pair<std::string, std::string>> help = {{"h", "print this help."},
                                                             {"s", "source file."},
                                                             {"o", "output file."},
                                                             {"i", "include flag e.g. \"sub\""}};
    Utils::Command::setCommandLineArguments(argc, argv);
    Utils::Command::setCommandLineHelp(help);

    if (Utils::Command::isArgGiven("h"))
    {
        print_help();
    }
    else if (!Utils::Command::isArgGiven("s"))
    {
        Utils::Console::error("no source file given!");
        print_help();
    }
    else if (!Utils::Command::isArgGiven("o"))
    {
        Utils::Console::error("no output file given!");
        print_help();
    }
    else if (!Utils::Command::isArgGiven("i"))
    {
        Utils::Console::warning("no include flag given, using default.", "sub");
        Utils::Command::getArg("i").value = "sub";
    }

    std::vector<std::string> files{
            Utils::Command::getArg("s").value,
            Utils::Command::getArg("o").value,
    };

    if (files[0] == files[1])
    {
        Utils::Console::error("source and output file cannot be the same!", files, true);
        print_help();
    }

    std::vector<std::string> arguments = {
            std::string("source file:\t").append(files[0]),
            std::string("output file:\t").append(files[1]),
            std::string("convert pdf:\t").append((Utils::Command::isArgGiven("c") ? "yes" : "no")),
            std::string("include flag:\t!").append(Utils::Command::getArg("i").value)};

    Utils::Console::debug("Program start", arguments, true);
}

void substitute()
{
    std::string flag = std::string("!").append(Utils::Command::getArg("i").value);
    std::string source_doc = Utils::Command::getArg("s").value;
    std::string output_doc = Utils::Command::getArg("o").value;

    std::vector<std::string> source_lines;
    if (!Utils::FileIO::readFile(source_doc, source_lines, false))
    {
        Utils::Console::error("could not read file", source_doc);
        exit(EXIT_SUCCESS);
    }


    std::string local_path = source_doc.substr(0, source_doc.find_last_of('/')+1);

    for (std::size_t i = 0; i < source_lines.size(); ++i)
    {
        // no flag -> add line to file.
        if (source_lines[i].find(flag) == std::string::npos)
        {
            if (!Utils::FileIO::writeToFile(output_doc, std::vector<std::string>{source_lines[i]}, (i != 0), false))
            {
                Utils::Console::error("could not write to file", output_doc);
            }
        }
        // flag detected -> insert included file.
        else
        {
            std::string path = local_path + Utils::Misc::divide(source_lines[i], ' ')[1];
            std::vector<std::string> include_file_lines;
            if (!Utils::FileIO::readFile(path, include_file_lines, false))
            {
                Utils::Console::error("could not read file", path);
                exit(EXIT_SUCCESS);
            }
            if (!Utils::FileIO::writeToFile(output_doc, include_file_lines, true, false))
            {
                Utils::Console::error("could not write to file", output_doc);
                exit(EXIT_SUCCESS);
            }

            Utils::Console::debug("included ", path);
        }
    }
}

int main(int argc, char** argv)
{
    Utils::Console::info("checking input");

    check_input(argc, argv);
    substitute();

    Utils::Console::info("program complete");
    return EXIT_SUCCESS;
}
