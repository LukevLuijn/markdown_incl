//
// Created by luke on 04-10-21.
//

#include "Convert.h"

#include <map>

#include "Console.h"
#include "FileIO.h"
#include "Misc.h"

namespace Program
{
    /* static */ bool Convert::convert_document(const std::string& src, const std::string& out)
    {
        std::vector<std::string> lines;

        if (!manage_include(lines, "!sub", src))
        {
            return false;
        }
        manage_urls(lines, "!url");
        manage_assets(lines, "!assets");

        if (!Utils::FileIO::writeToFile(out, lines, false, false))
        {
            Utils::Console::error("could not write to file", out);
            return false;
        }

        return true;
    }
    /* static */ bool Convert::get_lines(const std::string& path, const std::string& flag,
                                         std::vector<std::string>& buffer, uint16_t& nFiles)
    {
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
                if (!get_lines(include_path, flag, buffer, nFiles))
                {
                    return false;
                }
                ++nFiles;
            }
        }
        return true;
    }

    /* static */ void Convert::manage_assets(std::vector<std::string>& lines, const std::string& target)
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
        std::vector<std::string> stats;
        stats.reserve(keywords.size());
        for (const auto& key : keywords)
        {
            stats.emplace_back(std::string(key.first).append("\tx").append(std::to_string(key.second)));
        }
        Utils::Console::debug("assets found", stats, true);
    }

    /* static */ void Convert::manage_urls(std::vector<std::string>& lines, const std::string& target)
    {
        std::vector<std::string> urls;
        uint16_t duplicates = 0;
        for (auto it = lines.begin(); it != lines.end();)
        {
            if (it->find(target) != std::string::npos)
            {
                std::string entry = it->substr(target.length() + 1, it->length() - target.length() + 1);
                if (std::find(urls.begin(), urls.end(), entry) == urls.end())
                {
                    urls.emplace_back(entry);
                }
                else
                {
                    ++duplicates;
                }
                lines.erase(it);
                continue;
            }
            ++it;
        }

        for (std::size_t i = 0; i < urls.size(); ++i)
        {
            urls[i] = std::string("| ").append(std::to_string(i + 1)).append(" | ").append(urls[i]).append(" | ");
        }

        lines.emplace_back("\n# Bibliography\n");
        lines.emplace_back("\n|#|Source|");
        lines.emplace_back("|:---:|:---|");

        lines.insert(lines.end(), urls.begin(), urls.end());

        std::vector<std::string> params = {std::string(target).append("\t\tx").append(std::to_string(urls.size())),
                                           std::string("filtered").append("\tx").append(std::to_string(duplicates))};
        Utils::Console::debug("urls found", params, true);
    }

    /* static */ bool Convert::manage_include(std::vector<std::string>& buffer, const std::string& target,
                                              const std::string& source)
    {
        std::vector<std::string> source_lines;
        if (!Utils::FileIO::readFile(source, source_lines, false))
        {
            Utils::Console::error("could not read file", source);
            return false;
        }
        uint16_t nFiles = 0;
        if (!get_lines(source, target, buffer, nFiles))
        {
            return false;
        }
        Utils::Console::debug(
                "includes found",
                std::vector<std::string>{std::string(target).append("\t\tx").append(std::to_string(nFiles))},
                true);
        return true;
    }
}// namespace Program
