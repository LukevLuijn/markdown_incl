# Utils

## Command

This class gives the possibility to use commandline arguments and acces these arguments anywhere.

<details>
  <summary> methods </summary>

```c++        
/**
 * extract commandline arguments from given parameters
 *
 * @param argc argument count
 * @param argv argument value
 */
static void setCommandLineArguments(int argc, char* argv[]);
/**
 * check if argument with name: variable is given to program
 *
 * @param variable  name of argument
 * @return          true if argument is given
 */
static bool isArgGiven(const std::string& variable);
/**
 * get CommandLineArgument with name: variable
 *
 * @param   variable    name of requested CommandLineArgument
 * @return              CommandLineArgument with name: variable
 * @throws              std::invalid_argument if requested CommandLineArgument does not exist
 */
static CommandlineArgument& getArg(const std::string& variable);
/**
 * get CommandLineArgument with index: number
 *
 * @param number    index of requested CommandLineArgument
 * @return          CommandLineArgument with index: number
 * @throws          std::invalid_argument if requested CommandLineArgument does not exist
 */
static CommandlineArgument& getArg(uint64_t number);
/**
 * get CommandLineFiles
 *
 * @return the commandline files
 */
static std::vector<std::string>& getCommandLineFiles();
```

</details>

## Console

The Console class is essentially a logger. The class works with five levels (ERROR,TRANS,INFO,WARNING,DEBUG).
the user can set the verbosity level and/or (de)activate specific levels.

<details>
  <summary> methods </summary>

```c++        
/**
 * set max verbosity for program
 *
 * @param maxVerbosity [ERROR,TRANS,INFO,WARNING,DEBUG]
 */
static void setMaxVerbosity(uint8_t maxVerbosity = DEBUG);
/**
 * deactivate type
 *
 * @param type [ERROR,TRANS,INFO,WARNING,DEBUG]
 */
static void deactivateType(const Type& type);
/**
 * activate type
 *
 * @param type [ERROR,TRANS,INFO,WARNING,DEBUG]
 */
static void activateType(const Type& type);
/**
 * print error message
 *
 * @tparam T            type of argument list
 * @param message       message to print
 * @param vars          argument list
 * @param printAsList   true for numbered list, false for parameter style
 */
template<typename T>
static void error(const std::string& message, const std::vector<T>& vars,bool printAsList = false);
/**
 * print error message
 *
 * @tparam T        type of argument
 * @param message   message to print
 * @param var       argument
 */
template<typename T>
static void error(const std::string& message, T var);
/**
 * print error message
 *
 * @param message message to print
 */
static void error(const std::string& message);
/**
 * print info message
 *
 * @tparam T            type of argument list
 * @param message       message to print
 * @param vars          argument list
 * @param printAsList   true for numbered list, false for parameter style
 */
template<typename T>
static void info(const std::string& message, const std::vector<T>& vars, bool printAsList = false);
/**
 * print info message
 *
 * @tparam T        type of argument
 * @param message   message to print
 * @param var       argument
 */
template<typename T>
static void info(const std::string& message, T var);
/**
 * print info message
 *
 * @param message message to print
 */
static void info(const std::string& message);
/**
 * print warning message
 *
 * @tparam T            type of argument list
 * @param message       message to print
 * @param vars          argument list
 * @param printAsList   true for numbered list, false for parameter style
 */
template<typename T>
static void warning(const std::string& message, const std::vector<T>& vars, bool printAsList = false);
/**
 * print warning message
 *
 * @tparam T        type of argument
 * @param message   message to print
 * @param var       argument
 */
template<typename T>
static void warning(const std::string& message, T var);
/**
 * print warning message
 *
 * @param message message to print
 */
static void warning(const std::string& message);
/**
 * print debug message
 *
 * @tparam T            type of argument list
 * @param message       message to print
 * @param vars          argument list
 * @param printAsList   true for numbered list, false for parameter style
 */
template<typename T>
static void debug(const std::string& message, const std::vector<T>& vars, bool printAsList = false);
/**
 * print debug message
 *
 * @tparam T        type of argument
 * @param message   message to print
 * @param var       argument
 */
template<typename T>
static void debug(const std::string& message, T var);
/**
 * print debug message
 *
 * @param message message to print
 */
static void debug(const std::string& message);
/**
 * print transition message
 *
 * @param message   message to print
 * @param from      state transitioning from
 * @param to        state transitioning to
 */
static void transition(const std::string& message, const std::string& from, const std::string& to);
/**
 * convert string to colored string
 *
 * @param text      text to convert
 * @param color     color to convert to
 * @return          converted string
 */
static std::string convertToColor(const std::string& text, Color color);
```

</details>

<details>
  <summary> enums </summary>

### Type

```c++
enum Type
{
    ERROR,   /** @info verbosity: ERROR */
    TRANS,   /** @info verbosity: TRANSITION*/
    INFO,    /** @info verbosity: INFO*/
    WARNING, /** @info verbosity: WARNING*/
    DEBUG,   /** @info verbosity: DEBUG*/
};
```

### Color

```c++
enum Color
{
    NORMAL_BLACK   = 30, /** @info color code for normal black */
    NORMAL_RED     = 31, /** @info color code for normal red */
    NORMAL_GREEN   = 32, /** @info color code for normal green */
    NORMAL_YELLOW  = 33, /** @info color code for normal yellow*/
    NORMAL_BLUE    = 34, /** @info color code for normal blue */
    NORMAL_MAGENTA = 35, /** @info color code for normal magenta */
    NORMAL_CYAN    = 36, /** @info color code for normal cyan */
    NORMAL_WHITE   = 37, /** @info color code for normal white */
    BRIGHT_BLACK   = 90, /** @info color code for bright black */
    BRIGHT_RED     = 91, /** @info color code for bright red */
    BRIGHT_GREEN   = 92, /** @info color code for bright green */
    BRIGHT_YELLOW  = 93, /** @info color code for bright yellow*/
    BRIGHT_BLUE    = 94, /** @info color code for bright blue */
    BRIGHT_MAGENTA = 95, /** @info color code for bright magenta */
    BRIGHT_CYAN    = 96, /** @info color code for bright cyan */
    BRIGHT_WHITE   = 97  /** @info color code for bright white */
};
```

</details>

## FileIO

FiloIO gives easy access to creating, reading or removing directories and files.

<details>
  <summary> methods </summary>

```c++
/**
 * read content of file
 *
 * @param path      path to file
 * @param buffer    buffer for file content
 * @param fromRoot  true if path is from root, false for absolute
 * @param index     index >= 0 for specific line
 * @return          true if file was successfully read
 */
static bool readFile(Path path, Buffer buffer, bool fromRoot = true, int16_t index = -1);
/**
 * create new file
 *
 * @param path      path to new file
 * @param name      name of new file
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if file was successfully created
 */
static bool createFile(Path path, Name name, bool fromRoot = true);
/**
 * remove file
 *
 * @param path      path to file
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if file was successfully removed
 */
static bool removeFile(Path path, bool fromRoot = true);
/**
 * read content of directory
 *
 * @param path      path to directory
 * @param buffer    buffer for directory content
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if directory was successfully read
 */
static bool readDirectory(Path path, Buffer buffer, bool fromRoot = true);
/**
 * create new directory
 *
 * @param path      path to new directory
 * @param name      name of new directory
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if directory was successfully created
 */
static bool createDirectory(Path path, Name name, bool fromRoot = true);
/**
 * remove directory
 *
 * @param path      path to directory
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if directory was successfully removed
 */
static bool removeDirectory(Path path, bool fromRoot = true);
/**
 * write to file
 *
 * @param path      path to file
 * @param lines     lines to write
 * @param append    true if append, false if replace
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if writing was successful
 */
static bool writeToFile(Path path, Lines lines, bool append = true, bool fromRoot = true);
/**
 * check if element exists
 *
 * @param path      path to element
 * @param fromRoot  true if path is from root, false for absolute
 * @return          true if element exists
 */
static bool elementExist(Path path, bool fromRoot = true);
```

</details>

## Misc

pool of individual functions with different use cases.

<details>
  <summary> methods </summary>

```c++
/**
 * divide string by char
 *
 * @param str   string to divide
 * @param c     separator
 * @return      list of separated words
 */
static std::vector<std::string> divide (const std::string& str, char c);
```

</details>

