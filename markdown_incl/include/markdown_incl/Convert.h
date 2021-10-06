//
// Created by luke on 04-10-21.
//

#ifndef MARKDOWN_INCL_CONVERT_H
#define MARKDOWN_INCL_CONVERT_H

#include <map>
#include <string>
#include <vector>

namespace Program
{

    enum class Targets_e : uint8_t;
    struct Flag;

    class Convert
    {
    public:
        /**
         * @brief Convert given source document.
         *
         * @param src Path to source document.
         * @param out Path to output document.
         * @return true if conversion was successful, false if not.
         */
        static bool convert_document(const std::string& src, const std::string& out);

    private:
        /**
         * @brief Inserts page breaks after every header 1
         *
         * @if !page_break flag is set to true.
         *
         * @param lines Lines of document.
         * @param target Target for config.
         */
        static void handle_page_break(std::vector<std::string>& lines);
        /**
         * @brief Creates table of contents by searching for headers. Inserts toc at specified location [!insert_toc].
         *
         * @if !toc flag is set to true.
         *
         * @param lines Lines of document.
         * @param target Target for config.
         * @param has_chapter_numbers True if document uses chapter numbers.
         */
        static void handle_toc(std::vector<std::string>& lines, bool has_chapter_numbers);
        /**
         * @brief Adds chapter numbering to all headers and sub headers.
         *
         * @if !numbers flag is set to true.
         *
         * @param lines Lines of document
         * @param target Target for config.
         * @param toc_target Target for TOC @see handle_toc
         */
        static void handle_chapters(std::vector<std::string>& lines);
        /**
         * @brief Reads ignore flag from config. filters all (sub)lines from document with this flag.
         *
         * @if !ignore flag is set.
         *
         * @param lines Lines of document
         * @param target Target for config.
         */
        static void handle_ignores(std::vector<std::string>& lines);
        /**
         * @brief Automatically numbers all assets in order, multiple assets possible.
         *
         * @if !assets flag is set.
         *
         * @param lines Lines of document
         * @param target Target for config.
         */
        static void handle_assets(std::vector<std::string>& lines);
        /**
         * @brief   Scans all (sub)documents for !url flags,
         *          and places them at a specified location [!insert_url] on the output document.
         *
         * @if !url_insert flag is set.
         *
         * @param lines Lines of document
         * @param target Target for config.
         */
        static void handle_urls(std::vector<std::string>& lines);
        /**
         * @brief   Scans document for !sub flag, if found does the same of sub-document until no !sub flags are found
         *          all content of (sub)documents are written in order to output document.
         *
         * @param buffer Contains all lines from (sub)documents
         * @param target Target flag [!sub]
         * @param source Path to source document.
         * @return true if read was successful, false if not.
         */
        static bool handle_include(std::vector<std::string>& buffer, const std::string& source);
        /**
         * @brief   Puts all the lines in document in buffer,
         *          if flag is detected goes in this new document and does the same, recursively.
         *
         * @param path  Path to document.
         * @param flag  Flag to scan for.
         * @param buffer Buffer to write the lines to.
         * @param nFiles Amount of files scanned.
         * @return True if read(s) were successful, false if not.
         */
        static bool get_lines(const std::string& path, const std::string& flag, std::vector<std::string>& buffer,
                              uint16_t& nFiles);
        /**
         * @brief   Searches and reads config flag for specific target, parses these parameters and puts them in buffer.
         *          If flag is found it erases that line.
         *
         * @param target Target flag is searches for.
         * @param buffer Buffer where the found parameters are written to.
         * @param lines The lines it reads from.
         * @return True if target was found and parsed, false if not.
         */
        static bool get_keywords(const Targets_e& target, std::vector<std::string>& buffer,
                                 std::vector<std::string>& lines);
        /**
         * @brief   Looks for flag [!insert_<target>] erases this line and places content of buffer at this location.
         *
         * @param target Target to search for.
         * @param buffer Buffer to write.
         * @param lines Lines to write buffer to.
         * @return True if target was found, false if not.
         */
        static bool insert_at_target(const Targets_e& target, const std::vector<std::string>& buffer,
                                     std::vector<std::string>& lines);

        static void format_table(std::vector<std::string>& lines, const std::size_t& start, const std::size_t& end);
        static void handle_tables(std::vector<std::string>& lines);

    private:
        static const std::vector<std::string> CHAPTERS;
        static const std::map<Targets_e, std::string> TARGETS;
    };

    enum class Targets_e : uint8_t
    {
        ASSET = 0,
        IGNORE = 1,
        URL_BASE = 2,
        URL_TITLE = 3,
        PAGE_BREAK = 4,
        TOC_ENABLE = 5,
        TOC_TITLE = 6,
        TOC_DEPTH = 7,
        CHAP_NUMBERS = 8,
        DOC_INSERT = 9,
        TOC_INSERT = 10,
        URL_INSERT = 11,
        TABLE_FORMAT = 12,
        TABLE_START = 13,
        TABLE_END = 14,
    };


}// namespace Program

#endif//MARKDOWN_INCL_CONVERT_H
