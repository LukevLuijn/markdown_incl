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
        static void print_help();
        static void print_examples();
        static void check_input(int argc, char** argv);
    };
}// namespace Program

#endif//MARKDOWN_INCL_SETUP_H
