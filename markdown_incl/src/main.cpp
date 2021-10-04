
#include <iostream>
#include <map>
#include <string>

#include "Command.h"
#include "Console.h"
#include "FileIO.h"
#include "Misc.h"

void print_help()
{
    std::vector<std::string> features =
            {
                    std::string("to include a markdown file set \"!sub\"-flag."),
                    std::string("to use automatic asset numbering set \"!assets\"-flag with associated keywords."),
                    std::string("to centralize all urls in one place (bottom of document) set for every url the \"!url\"-flag.")
            };

    std::string program_title = "markdown_incl";
    Utils::Command::help(program_title);
    Utils::Console::info("features of this program", features, true);
    exit(EXIT_SUCCESS);
}

void print_examples()
{
    std::string help_doc_path = "../docs/help.md";
    std::vector<std::string> lines;
    Utils::FileIO::readFile(help_doc_path, lines, false);

    for(std::string& line : lines)
    {
        if (line[0] == '!')
        {
            std::vector<std::string> words = Utils::Misc::divide(line, ' ');
            for (std::size_t i = 0; i < words.size(); ++i)
            {
                std::cout << Utils::Console::convertToColor(words[i], (i == 0) ? Utils::Console::BRIGHT_GREEN : Utils::Console::BRIGHT_YELLOW) << " ";
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

void check_input(int argc, char** argv)
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

bool get_lines(const std::string& path, const std::string& flag, std::vector<std::string>& buffer)
{
    auto getFileName = [](const std::string& path) -> std::string {
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
            if (!get_lines(include_path, flag, buffer))
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

void manage_assets(std::vector<std::string>& lines, const std::string& target)
{

    std::map<std::string, uint16_t> keywords;
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        if (it->find(target) != std::string::npos)
        {
            std::vector<std::string> words = Utils::Misc::divide(*it, ' ');
            for (std::size_t i = 1; i < words.size(); ++i)
            {
                keywords.insert(keywords.begin(), std::make_pair("!" + words[i], 1));
            }

            lines.erase(it);
            break;
        }
    }
    for (std::string& line : lines)
    {
        for (auto& key : keywords)
        {
            std::string::size_type pos = 0;
            while ((pos = line.find(key.first, pos)) != std::string::npos)
            {
                std::string replacement = std::string(static_cast<char>(toupper(key.first[1])) +
                                                      key.first.substr(2, key.first.length() - 1))
                                                  .append(" ")
                                                  .append(std::to_string(key.second));

                line.replace(pos, key.first.length(), replacement);

                pos += key.first.length();
                ++key.second;
            }
        }
    }
}

void manage_urls(std::vector<std::string>& lines, const std::string& target)
{
    std::vector<std::string> urls;
    for (auto it = lines.begin(); it != lines.end();)
    {
        if (it->find(target) != std::string::npos)
        {
            urls.emplace_back("- " + it->substr(target.length() + 1, it->length() - target.length() + 1));
            lines.erase(it);
        }
        else
        {
            ++it;
        }
    }
    lines.insert(lines.end(), urls.begin(), urls.end());
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
    if (!get_lines(source_doc, flag, buffer))
    {
        exit(EXIT_SUCCESS);
    }

    manage_assets(buffer, "!assets");
    manage_urls(buffer, "!url");

    if (!Utils::FileIO::writeToFile(output_doc, buffer, false, false))
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
