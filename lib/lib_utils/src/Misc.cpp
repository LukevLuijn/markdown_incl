//
// Created by luke on 20-09-21.
//

#include "Misc.h"

#include <sstream>

namespace Utils
{
    /* static */ std::vector<std::string> Misc::divide (const std::string& str, char c)
    {
        std::stringstream ss(str);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, c))
            tokens.emplace_back(token);
        return tokens;
    }

    /* static */ void Misc::print_progress(double percentage, uint16_t bar_size)
    {
        const std::string progress_string = std::string(bar_size, '#');
        auto val = static_cast<uint32_t>(percentage * 100);
        auto left_pad = static_cast<uint32_t>(percentage * bar_size);
        uint32_t right_pad = bar_size - left_pad;
        printf("\r%3d%% [%.*s%*s]", val, left_pad, progress_string.c_str(), right_pad, "");
        fflush(stdout);
    }
}