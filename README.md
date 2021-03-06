# cpp_argument_parser
# Classes
- [ArgumentParser](#argumentparser-is-lightweight-header-only-argument-parser-class)
- [CommandLine](#commandline)
# Tools
- [Code generator](#code-generator)
## ArgumentParser is lightweight header only argument parser class
- *Uses **std::string_view** underneath instead of copying* :bulb:
- *Requirement: **C++17*** :warning:
- [Example :information_source:](#example)

## Program Argument Syntax
[See POSIX Program Argument Syntax Conventions](https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html)
- Arguments are options if they begin with a hyphen delimiter (‘-’)
- Multiple options may follow a hyphen delimiter in a single token if the options do not take arguments. Thus, ‘-abc’ is equivalent to ‘-a -b -c’
- Option names are single alphanumeric characters
- Certain options require an argument
- An option and its argument may or may not appear as separate tokens. 
  (In other words, the whitespace separating them is optional.) 
  Thus, ‘-o foo’ and ‘-ofoo’ are equivalent.
- Options typically precede other non-option arguments
- Long options consist of ‘--’ followed by a name made of alphanumeric characters and dashes

## Getter functions
```c++
size_t pos
```
* *programName*() -> ```const char*```
* *empty*() -> ```bool```
* *size*() -> ```size_t```
* *front*() -> ```std::string_view```
* *back*() -> ```std::string_view```
* *back*(pos) -> ```std::string_view```
* *at*(pos) -> ```std::string_view```
* *[pos]* -> ```std::string_view```
## Option parser functions
```c++
const std::string& short_opt, const std::string& long_opt
```
* *has*(short_opt, long_opt)       -> ```bool```
* *get*(short_opt, long_opt)       -> ```std::optional< std::string_view >```
* *getString*(short_opt, long_opt) -> ```std::optional< std::string >```
* *getList*(short_opt, long_opt)   -> ```std::optional< vector< std::string > >```
* *getInt*(short_opt, long_opt, base = 10)    -> ```std::optional< int64_t >```
* *getDouble*(short_opt, long_opt) -> ```std::optional< double >```
* *getHex*(short_opt, long_opt)    -> ```std::optional< int64_t >```

# Example

```c++
#include <stdio.h>
#include "argument_parser.h"

int main(int argc, char** argv)
{
    ArgumentParser arguments(argc, argv);
    if( arguments.empty() || arguments.has("-h", "--help") )
    {
        printf("Usage: %s [OPTIONS]\n"
               "\nOptions:\n"
               "\t-m, --message [MSG]   Expects some string\n"
               "\t-i, --integer [VALUE] Expects integer value\n"
               "\t-d, --double=[VALUE]  Expects double value\n"
               "\t-x, --hex [VALUE]     Expects hexa value\n"
               "\t-s                    Short version option\n"
               "\t    --long            Long version option\n"
               "\t-o, --optional        It is optional\n"
                "\t-h, --help           Show this help message\n\n"
               "\t-l, --list            [value][delim=',']*\n"
               "\t                      Accepts a list of values separated by ','\n"
              , arguments.programName() );
        return 0;
    }

    const bool flag = arguments.has("-o","--optional");
    const bool short_version_only = arguments.has("-s", "");
    const bool long_version_only = arguments.has("", "--long");

    int64_t integer_value = 0;
    double double_value = 0.0;
    int64_t hexadecimal = 0;
    std::string hex_as_str;
    std::string message;
    ArgumentParser::stringview_list_t list;
    
    if( auto integer_option = arguments.getInt("-i", "--integer") )
    { integer_value = integer_option.value(); }

    if( auto double_option = arguments.getDouble("-d", "--double") )
    { double_value = double_option.value(); }

    if( auto hex_option = arguments.getHex("-x", "--hex") )
    { 
        hexadecimal = hex_option.value();
    }

    if( auto msg_option = arguments.get("-m", "--message") )
    { message = msg_option.value(); }

    if( auto list_option = arguments.getList("-l", "--list", ",") )
    { list = list_option.value(); }
   
    auto front = std::string(arguments.front());
    auto back = std::string(arguments.back());
    auto before_back = std::string(arguments.back(1));

    printf("front: \t\"%s\"\nback: \t\"%s\"\nback-1: \"%s\"\n\n"
          , front.c_str()
          , back.c_str()
          , before_back.c_str()
    );
    
    printf("Read values:\n"
           "\tinteger(%ld)\n"
           "\tdouble(%f)\n"
           "\thex(0x%lx)\n"
           "\tflag(%s)\n"
           "\tshort(%s)\n"
           "\tlong(%s)\n"
           "\tmessage: '%s'\n"
          , integer_value
          , double_value
          , hexadecimal
          , flag ? "true" : "false"
          , short_version_only ? "true" : "false"
          , long_version_only ? "true" : "false"
          , message.c_str());
    puts("List:");
    for( auto& element : list )
    {
        std::string value(element);
        printf("\t%s\n", value.c_str());
    }

    return 0;
}
```

# CommandLine
[:arrow_left: Classes](#classes)
- *Provides an event loop for parsing commands with arguments from standard input*
- *Requirement: **ArgumentParser***
- [Example :information_source:](#example-2)

## Member functions
```c++
const std::string& command_name
```
* *add(command_name, [handler_function](#handler-function-type))* -> ```void```
* *commandList() -> ```std::vector< std::string >```
* *run()* -> ```int```

### Handler function type
```c++
std::function<int (const ArgumentParser&)>
```
- Input parameter is a reference to an ArgumentParser object containing the command arguments
- Returning other than zero from a handler function tells the event loop to quit

## Example 2
```c++
#include <stdio.h>
#include <ctime>
#include "command_line.h"

int main(int argc, char** argv)
{
    CommandLine cmd;

    cmd.add("time", [&cmd](const ArgumentParser& args) -> int
    {
        std::time_t result = std::time(nullptr);
        printf("%s", std::asctime(std::localtime(&result)));
        return 0;
    });

    cmd.add("help", [&cmd](const ArgumentParser& args) -> int
    {
        printf("Available commands:\n");
        auto cmd_list = cmd.commandList();
        for(auto& cmd_name : cmd_list)
        {
            printf("\t%s [OPTIONS]\n", cmd_name.c_str());
        }
        return 0;
    });

    return cmd.run();
}
```

# Code generator
[:arrow_left: Tools](#tools)
 
