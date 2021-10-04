//
// Created by luke on 04-10-21.
//

#ifndef MARKDOWN_INCL_CONVERT_H
#define MARKDOWN_INCL_CONVERT_H

#include <string>
#include <vector>

namespace Program
{
    class Convert
    {
    public:
        static bool convert_document(const std::string& src, const std::string& out);

    private:
        static void manage_toc(std::vector<std::string>& lines, const std::string& target);
        static void manage_chapters(std::vector<std::string>& lines, const std::string& target);
        static void manage_ignores(std::vector<std::string>& lines, const std::string& target);
        static void manage_assets(std::vector<std::string>& lines, const std::string& target);
        static void manage_urls(std::vector<std::string>& lines, const std::string& target);
        static bool manage_include(std::vector<std::string>& buffer, const std::string& target,
                                   const std::string& source);

        static bool get_lines(const std::string& path, const std::string& flag, std::vector<std::string>& buffer,
                              uint16_t& nFiles);
        static bool get_keywords(const std::string& target, std::vector<std::string>& buffer,
                                 std::vector<std::string>& lines);

    private:
        static std::vector<std::string> chapter_icon;
    };
}// namespace Program

#endif//MARKDOWN_INCL_CONVERT_H
