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
    /* static */ std::vector<std::string> Convert::chapter_icon = {"# ", "## ", "### ", "#### "};

    /* static */ bool Convert::convert_document(const std::string& src, const std::string& out)
    {
        std::vector<std::string> lines;

        if (!manage_include(lines, "!sub", src))
        {
            return false;
        }
        manage_ignores(lines, "!ignore");
        manage_urls(lines, "!url");
        manage_assets(lines, "!assets");
        manage_page_break(lines, "!page_break");
        manage_chapters(lines, "!numbers", "!toc");


        if (!Utils::FileIO::writeToFile(out, lines, false, false))
        {
            Utils::Console::error("could not write to file", out);
            return false;
        }

        return true;
    }
    /* static */ void Convert::manage_page_break(std::vector<std::string>& lines, const std::string& target)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(target, buffer, lines) || buffer[0] == "false")
        {
            // no keys found, nothing to do here.
            return;
        }

        std::string page_break = "\n<div style=\"page-break-after: always;\"></div>\n";
        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].substr(0, chapter_icon[0].size()) == chapter_icon[0])
            {
                lines.insert(lines.begin() + static_cast<int64_t>(i), page_break);
                ++i;
            }
        }
    }

    /* static */ void Convert::manage_ignores(std::vector<std::string>& lines, const std::string& target)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(target, buffer, lines))
        {
            // no keys found, nothing to do here.
            return;
        }

        for (auto it = lines.begin(); it != lines.end();)
        {
            auto pos = it->find(buffer[0] + " ");
            if (pos == 0)
            {
                lines.erase(it);
                continue;
            }
            else if (pos != std::string::npos)
            {
                *it = it->substr(0, pos);
            }
            ++it;
        }
    }
    /* static */ void Convert::manage_assets(std::vector<std::string>& lines, const std::string& target)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(target, buffer, lines))
        {
            // no keys found, nothing to do here.
            return;
        }
        std::map<std::string, uint16_t> keywords;
        for (const std::string& key : buffer)
        {
            keywords["!" + key] = 1;
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
            stats.emplace_back(std::string("x").append(std::to_string(key.second).append("\t").append(key.first)));
        }
        Utils::Console::debug("assets found", stats, true);
    }
    /* static */ void Convert::manage_urls(std::vector<std::string>& lines, const std::string& target)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(target + "_title", buffer, lines))
        {
            buffer[0] = "Bibliography";
        }

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
        std::vector<std::string> header = {"# " + buffer[0], "\n|Index|Source|", "|:---:|:---|"};
        urls.insert(urls.begin(), header.begin(), header.end());

        if (!urls.empty())
        {
            std::vector<std::string> params = {
                    std::string("x").append(std::to_string(urls.size()).append("\t").append(target)),
                    std::string("x").append(std::to_string(duplicates)).append("\t").append("filtered")};
            Utils::Console::debug("urls found", params, true);

            insert_at_target(target, urls, lines);
        }
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
        std::string param = std::string("x").append(std::to_string(nFiles)).append("\t").append(target);
        Utils::Console::debug("includes found", std::vector<std::string>{param}, true);
        return true;
    }
    /* static */ void Convert::manage_chapters(std::vector<std::string>& lines, const std::string& target,
                                               const std::string& toc_target)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(target, buffer, lines) || (!buffer.empty() && buffer[0] == "false"))
        {
            manage_toc(lines, toc_target, false);
            // no keys found, nothing to do here.
            return;
        }

        auto get_number = [&](std::vector<uint16_t>& list, uint16_t current) -> std::string {
            ++list[current - 1];
            std::string index;
            for (std::size_t i = 0; i < current; ++i)
            {
                index.append(std::to_string(list[i])).append(".");
            }
            index.append(" ");
            for (std::size_t i = current; i < list.size(); ++i)
            {
                list[i] = 0;
            }

            return index;
        };

        std::vector<uint16_t> indices(4, 0);
        std::vector<uint16_t> counters(4, 0);
        for (std::string& line : lines)
        {
            for (std::size_t i = 0; i < chapter_icon.size(); ++i)
            {
                if (line.substr(0, chapter_icon[i].size()) == chapter_icon[i])
                {
                    line.insert(chapter_icon[i].size(), get_number(indices, static_cast<uint16_t>(i + 1)));
                    ++counters[i];
                }
            }
        }

        std::vector<std::string> params;
        for (std::size_t i = 0; i < counters.size(); ++i)
        {
            if (counters[i] != 0)
            {
                params.emplace_back(std::string("x")
                                            .append(std::to_string(counters[i]))
                                            .append("\theader ")
                                            .append(std::to_string(i + 1)));
            }
        }
        Utils::Console::debug("headers found", params, true);

        manage_toc(lines, toc_target, true);
    }
    /* static */ void Convert::manage_toc(std::vector<std::string>& lines, const std::string& target,
                                          bool has_chapter_numbers)
    {
        std::vector<std::string> key_buffer;
        if (!get_keywords(target, key_buffer, lines) || key_buffer[0] == "false")
        {
            // no keys found, nothing to do here.
            return;
        }
        if (key_buffer[1].empty())
        {
            key_buffer[1] = "Table of contents";
        }

        std::vector<std::pair<std::string, uint16_t>> chapters;
        uint16_t counter = 0;
        for (auto& line : lines)
        {
            for (std::size_t i = 0; i < chapter_icon.size(); ++i)
            {
                if (line.substr(0, chapter_icon[i].size()) == chapter_icon[i])
                {
                    std::string title = line.substr(chapter_icon[i].size(), line.size() - chapter_icon[i].size());
                    chapters.emplace_back(std::make_pair(title, i));
                    line.append(" <a name=\"chapter" + std::to_string(counter) + "\"></a>");
                    ++counter;
                }
            }
        }

        std::vector<std::string> toc = {std::string("# ").append(key_buffer[1]).append("\n")};
        for (std::size_t i = 0; i < chapters.size(); ++i)
        {
            std::vector<std::string> words = Utils::Misc::divide(chapters[i].first, ' ');
            std::string entry = std::string("- ")
                                        .append((has_chapter_numbers) ? words[0].substr(0, words[0].size() - 1) : "")
                                        .append(" [");

            for (auto it = words.begin() + ((has_chapter_numbers) ? 1 : 0); it != words.end(); ++it)
            {
                entry.append(*it).append((std::next(it) != words.end()) ? " " : "");
            }

            entry.append("](#chapter").append(std::to_string(i)).append(")");

            for (uint16_t tab = 0; tab < chapters[i].second; ++tab)
            {
                entry = std::string("\t").append(entry);
            }
            toc.emplace_back(entry);
        }
        insert_at_target(target, toc, lines);
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
    /* static */ bool Convert::get_keywords(const std::string& target, std::vector<std::string>& keywords,
                                            std::vector<std::string>& lines)
    {
        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].find(target) != std::string::npos)
            {
                uint64_t param_start = lines[i].find('=') + 1;
                uint64_t param_end = lines[i].find(';');
                std::string params = lines[i].substr(param_start, param_end - param_start);

                std::vector<std::string> words = Utils::Misc::divide(params, ',');
                keywords.insert(keywords.end(), words.begin(), words.end());

                lines.erase(lines.begin() + static_cast<int64_t>(i));
                return true;
            }
        }
        // target not found
        Utils::Console::warning("keyword flag not found", target);
        return false;
    }
    /* static */ bool Convert::insert_at_target(const std::string& target, const std::vector<std::string>& buffer,
                                                std::vector<std::string>& lines)
    {
        std::string insert_target = target.substr(0, 1).append("insert_").append(target.substr(1, target.length() - 1));

        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].find(insert_target) != std::string::npos)
            {
                lines.erase(lines.begin() + static_cast<long>(i));
                lines.insert(lines.begin() + static_cast<long>(i), buffer.begin(), buffer.end());
                return true;
            }
        }
        // target not found
        Utils::Console::warning("target insert flag not found", insert_target);
        return false;
    }
}// namespace Program
