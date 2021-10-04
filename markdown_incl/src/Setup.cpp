//
// Created by luke on 04-10-21.
//

#include "Setup.h"

#include <string>
#include <vector>

#include "Command.h"
#include "Console.h"
#include "FileIO.h"
#include "Misc.h"

namespace Program
{
    /* static */ void Setup::print_help()
    {
        std::vector<std::string> features = {
                std::string("to include a markdown file set \"!sub\"-flag."),
                std::string("to use automatic asset numbering set \"!assets\"-flag with associated keywords."),
                std::string("to centralize all urls in one place (bottom of document) set for every url the "
                            "\"!url\"-flag.")};

        std::string program_title = "markdown_incl";
        Utils::Command::help(program_title);
        Utils::Console::info("features of this program", features, true);
        exit(EXIT_SUCCESS);
    }

    /* static */ void Setup::print_examples()
    {
        std::string help_doc_path = "../docs/help.md";
        std::vector<std::string> lines;
        Utils::FileIO::readFile(help_doc_path, lines, false);

        for (std::string& line : lines)
        {
            if (line[0] == '!')
            {
                std::vector<std::string> words = Utils::Misc::divide(line, ' ');
                for (std::size_t i = 0; i < words.size(); ++i)
                {
                    std::cout << Utils::Console::convertToColor(words[i],
                                                                (i == 0) ? Utils::Console::BRIGHT_GREEN
                                                                         : Utils::Console::BRIGHT_YELLOW)
                              << " ";
                }
                std::cout << std::endl;
            }
            else
            {
                std::cout << line << std::endl;
            }
        }
        exit(EXIT_SUCCESS);
    }

    /* static */ void Setup::check_input(int argc, char** argv)
    {
        std::vector<std::pair<std::string, std::string>> help = {{"h", "print this help."},
                                                                 {"e", "show examples."},
                                                                 {"s", "source file."},
                                                                 {"o", "output file."}};
        Utils::Command::setCommandLineArguments(argc, argv);
        Utils::Command::setCommandLineHelp(help);

        if (Utils::Command::isArgGiven("h"))
        {
            print_help();
        }
        else if (Utils::Command::isArgGiven("e"))
        {
            print_examples();
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

        std::vector<std::string> files{
                Utils::Command::getArg("s").value,
                Utils::Command::getArg("o").value,
        };

        if (files[0] == files[1])
        {
            Utils::Console::error("source and output file cannot be the same!", files, true);
            print_help();
        }

        std::vector<std::string> arguments = {std::string("source file:\t").append(files[0]),
                                              std::string("output file:\t").append(files[1])};

        Utils::Console::debug("Program start", arguments, true);
    }
}// namespace Program