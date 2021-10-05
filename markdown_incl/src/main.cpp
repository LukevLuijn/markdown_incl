
#include "Command.h"
#include "Console.h"
#include "Convert.h"
#include "Setup.h"

int main(int argc, char** argv)
{
    Utils::Console::info("checking input..");
    Utils::Console::setMaxVerbosity(Utils::Console::INFO);

    Program::Setup::check_input(argc, argv);

    std::string source = Utils::Command::getArg("s").value;
    std::string output = Utils::Command::getArg("o").value;

    (Program::Convert::convert_document(source, output)) ? Utils::Console::info("conversion success")
                                                         : Utils::Console::error("conversion error");
    return EXIT_SUCCESS;
}
