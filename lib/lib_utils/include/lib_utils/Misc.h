//
// Created by luke on 22-08-21.
//

#ifndef DAY_XX_MISC_H
#define DAY_XX_MISC_H

#include <iomanip>
#include <ios>
#include <string>
#include <vector>

namespace Utils
{
    class [[maybe_unused]] Misc
    {
    public:
        /**
         * divide string by char
         *
         * @param str   string to divide
         * @param c     separator
         * @return      list of separated words
         */
        [[maybe_unused]] static std::vector<std::string> divide (const std::string& str, char c);
        [[maybe_unused]] static void print_progress(double percentage, uint16_t bar_size);

        template<typename T>
        [[maybe_unused]] static std::string precision_cast(T num, uint16_t precision)
        {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision) << num;
            return ss.str();
        }


    };
}

#endif//DAY_XX_MISC_H
