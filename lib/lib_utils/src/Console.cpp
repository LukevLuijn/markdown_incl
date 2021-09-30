//
// Created by luke on 29-09-21.
//

#include "Console.h"

namespace Utils
{
    /* static */ uint8_t Console::max_verbosity = Console::DEBUG;
    /* static */ std::vector<Console::Type> Console::not_active_types = std::vector<Console::Type>{};

    /* static */ void Console::setMaxVerbosity(uint8_t maxVerbosity)
    {
        max_verbosity = maxVerbosity;
    }
    /* static */ void Console::deactivateType(const Type& type)
    {
        if (std::find(not_active_types.begin(), not_active_types.end(), type) == not_active_types.end())
        {
            not_active_types.emplace_back(type);
        }
    }
    /* static */ void Console::activateType(const Type& type)
    {
        not_active_types.erase(std::remove(not_active_types.begin(), not_active_types.end(), type),
                               not_active_types.end());
    }
    /* static */ void Console::error(const std::string& message)
    {
        if (isTypeActive(ERROR))
        {
            print(generateMessageString(ERROR, message, std::vector<int>{}));
        }
    }
    /* static */ void Console::info(const std::string& message)
    {
        if (isTypeActive(INFO))
        {
            print(generateMessageString(INFO, message, std::vector<int>{}));
        }
    }
    /* static */ void Console::warning(const std::string& message)
    {
        if (isTypeActive(WARNING))
        {
            print(generateMessageString(WARNING, message, std::vector<int>{}));
        }
    }
    /* static */ void Console::debug(const std::string& message)
    {
        if (isTypeActive(DEBUG))
        {
            print(generateMessageString(DEBUG, message, std::vector<int>{}));
        }
    }
    /* static */ void Console::transition(const std::string& message, const std::string& from, const std::string& to)
    {
        if (!isTypeActive(TRANS))
            return;

        std::string transMessage = generateMessageString(TRANS, message, std::vector<int>{});

        std::string transVars = convertToColor(" [", BRIGHT_CYAN);

        transVars += convertToColor(from, NORMAL_RED);
        transVars += convertToColor(" -> ", BRIGHT_CYAN);
        transVars += convertToColor(to, NORMAL_GREEN);

        transVars += convertToColor("]", BRIGHT_CYAN);

        print(transMessage + transVars);
    }
    /* static */ std::string Console::convertToColor(const std::string& text, Color color)
    {
        return "\x1B[" + std::to_string(color) + "m" + text + "\033[0m";
    }
    /* static */ std::string Console::getTypeMessage(const Type& messageType)
    {
        switch (messageType)
        {
            case ERROR:
                return convertToColor("[ERROR]\t", BRIGHT_RED);
            case TRANS:
                return convertToColor("[TRANS]\t", BRIGHT_MAGENTA);
            case INFO:
                return convertToColor("[INFO]\t", BRIGHT_GREEN);
            case WARNING:
                return convertToColor("[WARN]\t", BRIGHT_YELLOW);
            case DEBUG:
                return convertToColor("[DEBUG]\t", BRIGHT_MAGENTA);
            default:
                return convertToColor("[UNDEFINED]\t", NORMAL_BLACK);
        }
    }
    /* static */ std::string Console::generateMessageString(const Type& messageType, const std::string& message, const std::vector<std::string>& vars)
    {
        std::string varString = convertToColor("(", BRIGHT_CYAN);
        for_each(vars.begin(), vars.end(), [&](const std::string& var) {
          varString += convertToColor(var, NORMAL_WHITE) + ", ";
        });
        varString = varString.substr(0, varString.length() - 2) + convertToColor(")", BRIGHT_CYAN);
        std::string totString = getTypeMessage(messageType) + message + ((vars.empty()) ? "" : " " + varString);
        return totString;
    }
    /* static */ std::string Console::generateMessageStringAsList(const Type& messageType, const std::string& message,const std::vector<std::string>& vars)
    {
        std::string varString;
        for (std::size_t i = 0; i < vars.size(); ++i)
        {
            std::string indexStr = convertToColor("[", BRIGHT_CYAN) +
                                   convertToColor(std::to_string(i), NORMAL_WHITE) +
                                   convertToColor("]", BRIGHT_CYAN);

            varString += "  " + indexStr + convertToColor(" -> ", BRIGHT_YELLOW);
            varString += convertToColor(vars[i], NORMAL_WHITE) + ((i != vars.size() - 1) ? "\n" : "");
        }

        std::string totString = getTypeMessage(messageType) + convertToColor(message, BRIGHT_WHITE) + "\n";
        totString += ((vars.empty()) ? "" : varString);
        return totString;
    }
    /* static */ void Console::print(const std::string& message)
    {
        std::cout << message << std::endl;
    }
    /* static */ bool Console::isTypeActive(const Type& type)
    {
        if (max_verbosity < type ||
            std::find(not_active_types.begin(), not_active_types.end(), type) != not_active_types.end())
        {
            return false;
        }
        return true;
    }
}// namespace Utils