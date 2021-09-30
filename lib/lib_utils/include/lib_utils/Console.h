//
// Created by luke on 10-08-21.
//

#ifndef LIB_UTILS_CONSOLE_H
#define LIB_UTILS_CONSOLE_H

#include <string>
#include <vector>

#include <algorithm>
#include <iomanip>
#include <iostream>

namespace Utils
{
    class Console
    {
    public:
        enum Type
        {
            ERROR,   /** @info verbosity: ERROR */
            TRANS,   /** @info verbosity: TRANSITION*/
            INFO,    /** @info verbosity: INFO*/
            WARNING, /** @info verbosity: WARNING*/
            DEBUG,   /** @info verbosity: DEBUG*/
        };

        enum Color
        {
            NORMAL_BLACK [[maybe_unused]] = 30,   /**< @info color code for normal black */
            NORMAL_RED [[maybe_unused]] = 31,     /**< @info color code for normal red */
            NORMAL_GREEN [[maybe_unused]] = 32,   /**< @info color code for normal green */
            NORMAL_YELLOW [[maybe_unused]] = 33,  /**< @info color code for normal yellow*/
            NORMAL_BLUE [[maybe_unused]] = 34,    /**< @info color code for normal blue */
            NORMAL_MAGENTA [[maybe_unused]] = 35, /**< @info color code for normal magenta */
            NORMAL_CYAN [[maybe_unused]] = 36,    /**< @info color code for normal cyan */
            NORMAL_WHITE [[maybe_unused]] = 37,   /**< @info color code for normal white */
            BRIGHT_BLACK [[maybe_unused]] = 90,   /**< @info color code for bright black */
            BRIGHT_RED [[maybe_unused]] = 91,     /**< @info color code for bright red */
            BRIGHT_GREEN [[maybe_unused]] = 92,   /**< @info color code for bright green */
            BRIGHT_YELLOW [[maybe_unused]] = 93,  /**< @info color code for bright yellow*/
            BRIGHT_BLUE [[maybe_unused]] = 94,    /**< @info color code for bright blue */
            BRIGHT_MAGENTA [[maybe_unused]] = 95, /**< @info color code for bright magenta */
            BRIGHT_CYAN [[maybe_unused]] = 96,    /**< @info color code for bright cyan */
            BRIGHT_WHITE [[maybe_unused]] = 97    /**< @info color code for bright white */
        };

    private:
        static uint8_t max_verbosity;
        static std::vector<Type> not_active_types;

    public:
        /**
         * set max verbosity for program
         *
         * @param maxVerbosity [ERROR,TRANS,INFO,WARNING,DEBUG]
         */
        [[maybe_unused]] static void setMaxVerbosity(uint8_t maxVerbosity = DEBUG);
        /**
         * deactivate type
         *
         * @param type [ERROR,TRANS,INFO,WARNING,DEBUG]
         */
        [[maybe_unused]] static void deactivateType(const Type& type);
        /**
         * activate type
         *
         * @param type [ERROR,TRANS,INFO,WARNING,DEBUG]
         */
        [[maybe_unused]] static void activateType(const Type& type);
        /**
         * print error message
         *
         * @tparam T            type of argument list
         * @param message       message to print
         * @param vars          argument list
         * @param printAsList   true for numbered list, false for parameter style
         */
        template<typename T>
        [[maybe_unused]] static void error(const std::string& message, const std::vector<T>& vars,
                                           bool printAsList = false)
        {
            if (isTypeActive(ERROR))
            {
                const std::string gen_msg = (printAsList) ? generateMessageStringAsList(ERROR, message, vars)
                                                          : generateMessageString(ERROR, message, vars);
                print(gen_msg);
            }
        }
        /**
         * print error message
         *
         * @tparam T        type of argument
         * @param message   message to print
         * @param var       argument
         */
        template<typename T>
        [[maybe_unused]] static void error(const std::string& message, T var)
        {
            if (isTypeActive(ERROR))
            {
                print(generateMessageString(ERROR, message, std::vector<T>{var}));
            }
        }
        /**
         * print error message
         *
         * @param message message to print
         */
        [[maybe_unused]] static void error(const std::string& message);
        /**
         * print info message
         *
         * @tparam T            type of argument list
         * @param message       message to print
         * @param vars          argument list
         * @param printAsList   true for numbered list, false for parameter style
         */
        template<typename T>
        [[maybe_unused]] static void info(const std::string& message, const std::vector<T>& vars,
                                          bool printAsList = false)
        {
            if (isTypeActive(INFO))
            {
                const std::string gen_msg = (printAsList) ? generateMessageStringAsList(INFO, message, vars)
                                                          : generateMessageString(INFO, message, vars);
                print(gen_msg);
            }
        }
        /**
         * print info message
         *
         * @tparam T        type of argument
         * @param message   message to print
         * @param var       argument
         */
        template<typename T>
        [[maybe_unused]] static void info(const std::string& message, T var)
        {
            if (isTypeActive(INFO))
            {
                print(generateMessageString(INFO, message, std::vector<T>{var}));
            }
        }
        /**
         * print info message
         *
         * @param message message to print
         */
        [[maybe_unused]] static void info(const std::string& message);
        /**
         * print warning message
         *
         * @tparam T            type of argument list
         * @param message       message to print
         * @param vars          argument list
         * @param printAsList   true for numbered list, false for parameter style
         */
        template<typename T>
        [[maybe_unused]] static void warning(const std::string& message, const std::vector<T>& vars,
                                             bool printAsList = false)
        {
            if (isTypeActive(WARNING))
            {
                const std::string gen_msg = (printAsList) ? generateMessageStringAsList(WARNING, message, vars)
                                                          : generateMessageString(WARNING, message, vars);
                print(gen_msg);
            }
        }
        /**
         * print warning message
         *
         * @tparam T        type of argument
         * @param message   message to print
         * @param var       argument
         */
        template<typename T>
        [[maybe_unused]] static void warning(const std::string& message, T var)
        {
            if (isTypeActive(WARNING))
            {
                print(generateMessageString(WARNING, message, std::vector<T>{var}));
            }
        }
        /**
         * print warning message
         *
         * @param message message to print
         */
        [[maybe_unused]] static void warning(const std::string& message);
        /**
         * print debug message
         *
         * @tparam T            type of argument list
         * @param message       message to print
         * @param vars          argument list
         * @param printAsList   true for numbered list, false for parameter style
         */
        template<typename T>
        [[maybe_unused]] static void debug(const std::string& message, const std::vector<T>& vars,
                                           bool printAsList = false)
        {
            if (isTypeActive(DEBUG))
            {
                const std::string gen_msg = (printAsList) ? generateMessageStringAsList(DEBUG, message, vars)
                                                          : generateMessageString(DEBUG, message, vars);
                print(gen_msg);
            }
        }
        /**
         * print debug message
         *
         * @tparam T        type of argument
         * @param message   message to print
         * @param var       argument
         */
        template<typename T>
        [[maybe_unused]] static void debug(const std::string& message, T var)
        {
            if (isTypeActive(DEBUG))
            {
                print(generateMessageString(DEBUG, message, std::vector<T>{var}));
            }
        }
        /**
         * print debug message
         *
         * @param message message to print
         */
        [[maybe_unused]] static void debug(const std::string& message);
        /**
         * print transition message
         *
         * @param message   message to print
         * @param from      state transitioning from
         * @param to        state transitioning to
         */
        [[maybe_unused]] static void transition(const std::string& message, const std::string& from,
                                                const std::string& to);
        /**
         * convert string to colored string
         *
         * @param text      text to convert
         * @param color     color to convert to
         * @return          converted string
         */
        static std::string convertToColor(const std::string& text, Color color);

    private:
        static std::string getTypeMessage(const Type& messageType);

        template<typename T>
        static std::string generateMessageString(const Type& messageType, const std::string& message,
                                                 const std::vector<T>& vars)
        {
            std::vector<std::string> varsAsString;
            for_each(vars.begin(), vars.end(), [&](T var) {
                varsAsString.template emplace_back(convertToString(var));
            });
            return generateMessageString(messageType, message, varsAsString);
        }
        static std::string generateMessageString(const Type& messageType, const std::string& message,
                                                 const std::vector<std::string>& vars);
        template<typename T>
        static std::string generateMessageStringAsList(const Type& messageType, const std::string& message,
                                                       const std::vector<T>& vars)
        {
            std::vector<std::string> varsAsString;
            for_each(vars.begin(), vars.end(), [&](T var) {
                varsAsString.template emplace_back(convertToString(var));
            });
            return generateMessageStringAsList(messageType, message, varsAsString);
        }
        static std::string generateMessageStringAsList(const Type& messageType, const std::string& message,
                                                       const std::vector<std::string>& vars);
        template<typename T>
        static std::string convertToString(T var)
        {
            std::string valueString;
            if (std::is_same<T, bool>::value)
            {
                valueString = (static_cast<bool>(var)) ? convertToColor("true", NORMAL_GREEN)
                                                       : convertToColor("false", NORMAL_RED);
            }
            else if (std::is_same<T, float>::value || std::is_same<T, double>::value)
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << var;
                valueString = convertToColor(stream.str(), NORMAL_WHITE);
            }
            else
            {
                try
                {
                    std::ostringstream oss;
                    oss << var;
                    valueString = convertToColor(oss.str(), NORMAL_WHITE);
                }
                catch (...)
                {
                    valueString = convertToColor("[undefined]", NORMAL_WHITE);
                }
            }
            return valueString;
        }
        static void print(const std::string& message);
        static bool isTypeActive(const Type& type);
    };
}// namespace Utils

#endif //LIB_UTILS_CONSOLE_H
