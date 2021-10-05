//
// Created by luke on 04-10-21.
//

#ifndef MARKDOWN_INCL_SETUP_H
#define MARKDOWN_INCL_SETUP_H

namespace Program
{
    class Setup
    {
    public:
        /**
         * @brief Print help to console
         */
        static void print_help();
        /**
         * @brief Print examples [docs/help.md] to console
         */
        static void print_examples();
        /**
         * @brief Check if given parameters are correct.
         *
         * @param argc Argument count.
         * @param argv Argument value.
         */
        static void check_input(int argc, char** argv);
    };
}// namespace Program

#endif//MARKDOWN_INCL_SETUP_H
