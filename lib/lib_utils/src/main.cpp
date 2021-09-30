//
// Created by luke on 22-08-21.
//

#include <iostream>

#include "Command.h"
#include "Console.h"
#include "FileIO.h"

bool Command_test(int argc, char** argv);
bool Console_Test();
bool File_test();

int main(int argc, char** argv)
{
    if (!Console_Test())
        return EXIT_SUCCESS;
    if (!Command_test(argc, argv))
        return EXIT_SUCCESS;
    if (!File_test())
        return EXIT_SUCCESS;

    std::string sepp = "==============================================";
    std::cout << "\n" << Utils::Console::convertToColor(sepp, Utils::Console::NORMAL_GREEN) << std::endl;
    Utils::Console::info("all test passed, library ready for use.");
    std::cout << Utils::Console::convertToColor(sepp, Utils::Console::NORMAL_GREEN) << std::endl;

    return 0;
}

bool Command_test(int argc, char** argv)
{
    std::string var = "test", val = "test";
    Utils::Command::setCommandLineArguments(argc, argv);

    if (Utils::Command::isArgGiven(var))
    {
        if (Utils::Command::getArg(var).value == val)
        {
            return true;
        }
        else
        {
            Utils::Console::error("Command value for test argument does not equal", val);
            return false;
        }
    }
    else
    {
        Utils::Console::error("Command argument not detected", var);
        return false;
    }
}

bool Console_Test()
{
    using Utils::Console;

    std::vector<std::string> vals = {"these", "are", "different", "vars"};
    std::vector<bool> bools = {true, true, false};

    std::cout << __PRETTY_FUNCTION__ << " - " << Console::convertToColor("ERROR", Console::BRIGHT_RED) << std::endl;
    Console::error("error message; no variables");
    Console::error("error message; one variable", vals[0]);
    Console::error("error message; n variables", vals);
    Console::error("error message; booleans", bools);
    Console::error("error message; booleans", bools, true);

    std::cout << __PRETTY_FUNCTION__ << " - " << Console::convertToColor("INFO", Console::BRIGHT_GREEN) << std::endl;
    Console::info("info message; no variables");
    Console::info("info message; one variable", vals[0]);
    Console::info("info message; n variables", vals);
    Console::info("info message; booleans", bools);
    Console::info("info message; booleans", bools, true);


    std::cout << __PRETTY_FUNCTION__ << " - " << Console::convertToColor("WARNING", Console::BRIGHT_YELLOW)
              << std::endl;
    Console::warning("warn message; no variables");
    Console::warning("warn message; one variable", vals[0]);
    Console::warning("warn message; n variables", vals);
    Console::warning("warn message; booleans", bools);
    Console::warning("warn message; booleans", bools, true);

    std::cout << __PRETTY_FUNCTION__ << " - " << Console::convertToColor("DEBUG", Console::BRIGHT_MAGENTA) << std::endl;
    Console::debug("debug message; no variables");
    Console::debug("debug message; one variable", vals[0]);
    Console::debug("debug message; n variables", vals);
    Console::debug("debug message; booleans", bools);
    Console::debug("debug message; booleans", bools, true);

    std::string title =
            Console::convertToColor("C", Console::BRIGHT_BLUE) + Console::convertToColor("O", Console::BRIGHT_YELLOW) +
            Console::convertToColor("L", Console::BRIGHT_MAGENTA) + Console::convertToColor("O", Console::BRIGHT_CYAN) +
            Console::convertToColor("R", Console::BRIGHT_RED) + Console::convertToColor("S", Console::BRIGHT_GREEN);


    std::cout << __PRETTY_FUNCTION__ << " - " << title << std::endl;

    std::vector<std::string> colors_bright = {
            Console::convertToColor("GREEN", Console::BRIGHT_GREEN),
            Console::convertToColor("RED", Console::BRIGHT_RED),
            Console::convertToColor("CYAN", Console::BRIGHT_CYAN),
            Console::convertToColor("MAGENTA", Console::BRIGHT_MAGENTA),
            Console::convertToColor("YELLOW", Console::BRIGHT_YELLOW),
            Console::convertToColor("BLUE", Console::BRIGHT_BLUE),
            Console::convertToColor("BLACK", Console::BRIGHT_BLACK),
            Console::convertToColor("WHITE", Console::BRIGHT_WHITE),
    };

    std::cout << "[BRIGHT] = ";
    for (const std::string& color : colors_bright)
        std::cout << color << " ";
    std::cout << "\n";

    std::vector<std::string> colors_normal = {
            Console::convertToColor("GREEN", Console::NORMAL_GREEN),
            Console::convertToColor("RED", Console::NORMAL_RED),
            Console::convertToColor("CYAN", Console::NORMAL_CYAN),
            Console::convertToColor("MAGENTA", Console::NORMAL_MAGENTA),
            Console::convertToColor("YELLOW", Console::NORMAL_YELLOW),
            Console::convertToColor("BLUE", Console::NORMAL_BLUE),
            Console::convertToColor("BLACK", Console::NORMAL_BLACK),
            Console::convertToColor("WHITE", Console::NORMAL_WHITE),
    };

    std::cout << "[NORMAL] = ";
    for (const std::string& color : colors_normal)
        std::cout << color << " ";
    std::cout << "\n";
    return true;
}

bool File_test()
{
    auto readAndPrintFile = [&](const std::string& path) {
        std::vector<std::string> fileLines;
        Utils::FileIO::readFile(path, fileLines);
        Utils::Console::info("lines in file", fileLines, true);
    };

    std::vector<std::string> lines = {};
    std::string testDir = "/test_dir";
    std::string testFile = "/test_file.txt";

    using Utils::Console;
    using Utils::FileIO;

    if (FileIO::createDirectory("", testDir))
    {
        Console::warning("could not create test directory, could be because it already exists", testDir);
    }
    Console::info("test directory created", testDir);

    if (!FileIO::createFile(testDir, testFile))
    {
        Console::warning("could not create test file, could be because it already exists", testFile);
    }
    Console::info("test file created", testFile);
    FileIO::readDirectory(testDir, lines);
    Console::info("directory content", lines);

    if (!Utils::FileIO::writeToFile(testDir + testFile, std::vector<std::string>{"line written to new file."}, true))
    {
        Console::error("could not write to test file; [append first line]", testFile);
        return false;
    }
    Console::info("single line appended to file");
    readAndPrintFile(testDir + testFile);

    if (!FileIO::writeToFile(testDir + testFile, std::vector<std::string>{"line appended to new file."}, true))
    {
        Console::error("could not write to test file; [append second line]", testFile);
        return false;
    }
    Console::info("single line appended to file");
    readAndPrintFile(testDir + testFile);

    if (!FileIO::writeToFile(testDir + testFile, std::vector<std::string>{"content replaced with this line."}, false))
    {
        Console::error("could not write to test file; [replace content]", testFile);
        return false;
    }
    Console::info("replace file content with single line");
    readAndPrintFile(testDir + testFile);

    Console::info("true if " + testDir + testFile + " exists", FileIO::elementExist(testDir + testFile));

    if (!FileIO::removeFile(testDir + testFile))
    {
        Console::error("could not remove file", testFile);
        return false;
    }
    Console::info("test file removed", testFile);

    if (!FileIO::removeDirectory(testDir))
    {
        Console::error("could not remove file", testDir);
        return false;
    }
    Console::info("test directory removed", testDir);
    return true;
}