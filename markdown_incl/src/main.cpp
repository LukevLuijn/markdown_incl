
#include "Command.h"
#include "Console.h"

#include "Convert.h"
#include "Setup.h"

/**
 *  TODO
 *
 *  apa generated sources seem to be split during auto format.
 *  make separate flags for inserting TOC and Source list.
 */


int main(int argc, char** argv)
{
    Utils::Console::info("checking input..");
    Program::Setup::check_input(argc, argv);

    std::string source = Utils::Command::getArg("s").value;
    std::string output = Utils::Command::getArg("o").value;

    (Program::Convert::convert_document(source, output)) ? Utils::Console::info("conversion successful")
                                                         : Utils::Console::error("document conversion error");
    return EXIT_SUCCESS;
}
