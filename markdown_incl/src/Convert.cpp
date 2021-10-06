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
    /* static */ const std::vector<std::string> Convert::CHAPTERS = {"# ", "## ", "### ", "#### ", "##### "};
    /* static */ const std::map<Targets_e, std::string> Convert::TARGETS = {
            {Targets_e::ASSET, "!asset"},
            {Targets_e::IGNORE, "!ignore"},
            {Targets_e::URL_BASE, "!url"},
            {Targets_e::URL_TITLE, "!title_url"},
            {Targets_e::PAGE_BREAK, "!page_break"},
            {Targets_e::TOC_ENABLE, "!enable_toc"},
            {Targets_e::TOC_TITLE, "!title_toc"},
            {Targets_e::TOC_DEPTH, "!depth_toc"},
            {Targets_e::CHAP_NUMBERS, "!chap_num"},
            {Targets_e::DOC_INSERT, "!sub"},
            {Targets_e::TOC_INSERT, "!insert_toc"},
            {Targets_e::URL_INSERT, "!insert_url"},
            {Targets_e::TABLE_FORMAT, "!table_format"},
            {Targets_e::TABLE_START, "!table_start"},
            {Targets_e::TABLE_END, "!table_end"},
    };

    /* static */ bool Convert::convert_document(const std::string& src, const std::string& out)
    {
        // buffer for lines read from file.
        std::vector<std::string> lines;

        /*
         * reads in all lines including all includes,
         * if this method returns false there was a read
         * error and the program terminates.
         */
        if (handle_include(lines, src))
        {
            handle_ignores(lines);   // handle !ignore flags in file.
            handle_urls(lines);      // handle !url flags in file.
            handle_assets(lines);    // handle all assets in file.
            handle_page_break(lines);// insert page breaks after every header 1.
            handle_chapters(lines);  // insert page numbers for every chapter, also creates toc.
            handle_tables(lines);    // formats tables.

            /*
             * writes all converted lines to output file.
             * if this method returns false there was a
             * read error and the program terminates.
             */
            if (!Utils::FileIO::writeToFile(out, lines, false, false))
            {
                Utils::Console::error("could not write to file", out);
                return false;
            }
            // read, conversion and write successful.
            return true;
        }
        Utils::Console::error("could not read from file", src);
        return false;
    }

    // main methods

    /* static */ void Convert::handle_tables(std::vector<std::string>& lines)
    {
        std::vector<std::string> key_buffer;
        if (!get_keywords(Targets_e::TABLE_FORMAT, key_buffer, lines) || key_buffer[0] == "false")
            return;

        uint16_t tables_formatted = 0;
        std::size_t start_index = 0, end_index = 0;
        for (std::size_t start = 0; start < lines.size();)
        {
            if (lines[start].find(TARGETS.at(Targets_e::TABLE_START)) != std::string::npos)
            {
                start_index = start + 1;
                for (std::size_t end = start; end < lines.size(); ++end)
                {
                    if (lines[end].find(TARGETS.at(Targets_e::TABLE_END)) != std::string::npos)
                    {
                        end_index = end - 1;
                        format_table(lines, start_index, end_index);

                        lines.erase(lines.begin() + static_cast<long>(start_index - 1));
                        lines.erase(lines.begin() + static_cast<long>(end_index));

                        start = end;
                        ++tables_formatted;
                        break;
                    }
                }
            }
            ++start;
        }

        std::vector<std::string> param = {
                std::to_string(tables_formatted).append("x\t").append("tables")};
        Utils::Console::debug("tables found", param, true);
    }
    /* static */ void Convert::handle_page_break(std::vector<std::string>& lines)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(Targets_e::PAGE_BREAK, buffer, lines) || buffer[0] == "false")
            return;

        std::string page_break = "\n<div style=\"page-break-after: always;\"></div>\n";
        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].substr(0, CHAPTERS[0].size()) == CHAPTERS[0])
            {
                lines.insert(lines.begin() + static_cast<int64_t>(i), page_break);
                ++i;
            }
        }
    }
    /* static */ void Convert::handle_ignores(std::vector<std::string>& lines)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(Targets_e::IGNORE, buffer, lines))
            return;

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
    /* static */ void Convert::handle_assets(std::vector<std::string>& lines)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(Targets_e::ASSET, buffer, lines))
            return;

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
    /* static */ void Convert::handle_urls(std::vector<std::string>& lines)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(Targets_e::URL_TITLE, buffer, lines))
        {
            buffer[0] = "Bibliography";
        }

        std::vector<std::string> urls;

        uint16_t duplicates = 0;
        for (auto it = lines.begin(); it != lines.end();)
        {
            if (it->find(TARGETS.at(Targets_e::URL_BASE)) != std::string::npos)
            {
                std::string entry = it->substr(TARGETS.at(Targets_e::URL_BASE).length() + 1,
                                               it->length() - TARGETS.at(Targets_e::URL_BASE).length() + 1);
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
                    std::string("x").append(
                            std::to_string(urls.size()).append("\t").append(TARGETS.at(Targets_e::URL_BASE))),
                    std::string("x").append(std::to_string(duplicates)).append("\t").append("filtered")};
            Utils::Console::debug("urls found", params, true);

            insert_at_target(Targets_e::URL_INSERT, urls, lines);
        }
    }
    /* static */ bool Convert::handle_include(std::vector<std::string>& buffer, const std::string& source)
    {
        std::vector<std::string> source_lines;
        if (!Utils::FileIO::readFile(source, source_lines, false))
        {
            return false;
        }
        uint16_t nFiles = 0;
        if (!get_lines(source, TARGETS.at(Targets_e::DOC_INSERT), buffer, nFiles))
        {
            return false;
        }
        std::string param =
                std::string("x").append(std::to_string(nFiles)).append("\t").append(TARGETS.at(Targets_e::DOC_INSERT));
        Utils::Console::debug("includes found", std::vector<std::string>{param}, true);
        return true;
    }
    /* static */ void Convert::handle_chapters(std::vector<std::string>& lines)
    {
        std::vector<std::string> buffer;
        if (!get_keywords(Targets_e::CHAP_NUMBERS, buffer, lines) || (!buffer.empty() && buffer[0] == "false"))
        {
            handle_toc(lines, false);
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
            for (std::size_t i = 0; i < CHAPTERS.size(); ++i)
            {
                if (line.substr(0, CHAPTERS[i].size()) == CHAPTERS[i])
                {
                    line.insert(CHAPTERS[i].size(), get_number(indices, static_cast<uint16_t>(i + 1)));
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

        handle_toc(lines, true);
    }
    /* static */ void Convert::handle_toc(std::vector<std::string>& lines, bool has_chapter_numbers)
    {
        std::vector<std::string> key_buffer;
        if (!get_keywords(Targets_e::TOC_ENABLE, key_buffer, lines) || key_buffer[0] == "false")
            return;

        std::string chapter = "Table of Contents";
        key_buffer.clear();
        if (get_keywords(Targets_e::TOC_TITLE, key_buffer, lines))
            chapter = key_buffer[0];

        auto depth = static_cast<uint16_t>(CHAPTERS.size());
        key_buffer.clear();
        if (get_keywords(Targets_e::TOC_DEPTH, key_buffer, lines))
            depth = static_cast<unsigned short>(std::stoi(key_buffer[0]));


        std::vector<std::pair<std::string, uint16_t>> chapters;
        uint16_t counter = 0;
        for (auto& line : lines)
        {
            for (std::size_t i = 0; i < CHAPTERS.size(); ++i)
            {
                if (line.substr(0, CHAPTERS[i].size()) == CHAPTERS[i])
                {
                    std::string title = line.substr(CHAPTERS[i].size(), line.size() - CHAPTERS[i].size());
                    chapters.emplace_back(std::make_pair(title, i));
                    line.append(" <a name=\"chapter" + std::to_string(counter) + "\"></a>");
                    ++counter;
                }
            }
        }

        std::vector<std::string> toc = {std::string("# ").append(chapter).append("\n")};

        for (std::size_t i = 0; i < chapters.size(); ++i)
        {
            if (chapters[i].second > depth - 1)
                continue;

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
        insert_at_target(Targets_e::TOC_INSERT, toc, lines);
    }

    // Utility functions

    /* static */ bool Convert::get_lines(const std::string& path, const std::string& flag,
                                         std::vector<std::string>& buffer, uint16_t& nFiles)
    {
        std::string local_path = path.substr(0, path.find_last_of('/') + 1);
        std::vector<std::string> source_lines;

        if (!Utils::FileIO::readFile(path, source_lines, false))
        {
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
    /* static */ bool Convert::get_keywords(const Targets_e& target, std::vector<std::string>& keywords,
                                            std::vector<std::string>& lines)
    {
        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].find(TARGETS.at(target)) != std::string::npos)
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
        Utils::Console::warning("keyword flag not found", TARGETS.at(target));
        return false;
    }
    /* static */ bool Convert::insert_at_target(const Targets_e& target, const std::vector<std::string>& buffer,
                                                std::vector<std::string>& lines)
    {
        //        std::string insert_target = target.substr(0, 1).append("insert_").append(target.substr(1, target.length() - 1));

        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            if (lines[i].find(TARGETS.at(target)) != std::string::npos)
            {
                lines.erase(lines.begin() + static_cast<long>(i));
                lines.insert(lines.begin() + static_cast<long>(i), buffer.begin(), buffer.end());
                return true;
            }
        }
        // target not found
        Utils::Console::warning("target insert flag not found", TARGETS.at(target));
        return false;
    }
    /* static */ void Convert::format_table(std::vector<std::string>& lines, const std::size_t& start,
                                            const std::size_t& end)
    {
        std::vector<std::string> table_lines(lines.begin() + static_cast<int64_t>(start),
                                             lines.begin() + static_cast<int64_t>(end));

        uint16_t n_cols = static_cast<uint16_t>(std::count(table_lines[0].begin(), table_lines[0].end(), '|')-1);
        std::vector<uint16_t> col_lengths(n_cols, 0);
        for (const std::string& line : table_lines)
        {
            std::vector<std::string> cols = Utils::Misc::divide(line.substr(1, line.length() - 2), '|');
            for (std::size_t i = 0; i < cols.size(); ++i)
            {
                if (cols[i].length() > col_lengths[i])
                {
                    col_lengths[i] = static_cast<uint16_t>(cols[i].length() + 2);
                }
            }
        }

        for (std::size_t line_index = start; line_index <= end; ++line_index)
        {
            std::vector<std::string> cols =
                    Utils::Misc::divide(lines[line_index].substr(1, lines[line_index].length() - 2), '|');
            std::string new_line = "| ";

            for (std::size_t i = 0; i < cols.size(); ++i)
            {
                if (cols[i].length() >= col_lengths[i])
                {
                    cols[i] = std::string(" ").append(cols[i]).append(" | ");
                }
                else
                {
                    cols[i].append(std::string(col_lengths[i] - (cols[i].length() + 1), ' ')).append("| ");
                }
                new_line.append(cols[i]);
            }
            lines[line_index] = new_line;
        }
    }

}// namespace Program
