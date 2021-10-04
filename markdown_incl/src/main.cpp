
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
            std::string("include flag:\t!").append(Utils::Command::getArg("i").value)};

    Utils::Console::debug("Program start", arguments, true);
}

bool getLines(const std::string& path, const std::string& flag, std::vector<std::string>& buffer)
{
    auto getFileName = [](const std::string& path) -> std::string
    {
        uint64_t start_pos = path.find_last_of('/') + 1;
        return path.substr(start_pos, path.length() - start_pos);
    };

    std::string local_path = path.substr(0, path.find_last_of('/') + 1);
    std::vector<std::string> source_lines;
    if (!Utils::FileIO::readFile(path, source_lines, false))
    {
        Utils::Console::error("could not read file", path);
        return false;
    }
    for (const std::string& line : source_lines)
    {
        if (line.find(flag) == std::string::npos)
        {
            buffer.emplace_back(line);
        }
        else
        {
            std::string include_path = local_path + Utils::Misc::divide(line, ' ')[1];
            if (!getLines(include_path, flag, buffer))
            {
                return false;
            }
            std::vector<std::string> params = {std::string("base: ").append(getFileName(path)),
                                               std::string("\tsub: ").append(getFileName(include_path))};
            Utils::Console::debug("file inserted", params);
        }
    }
    return true;
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
    std::vector<std::string> buffer;
    if (!getLines(source_doc, flag, buffer))
    {
        exit(EXIT_SUCCESS);
    }
    else if (!Utils::FileIO::writeToFile(output_doc, buffer, false, false))
    {
        Utils::Console::error("could not write to file", output_doc);
        exit(EXIT_SUCCESS);
    }

    Utils::Console::debug("substitution complete");
}

int main(int argc, char** argv)
{
    Utils::Console::info("checking input..");

    check_input(argc, argv);
    substitute();

    Utils::Console::info("program complete");
    return EXIT_SUCCESS;
}
