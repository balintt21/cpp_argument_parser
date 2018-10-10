# cpp_argument_parser
## ArgumentParser is lightweight header only argument parser class
It uses std::string_view under the hood if compiled with c++17, what makes it a good choice for programs that are expecting a large number of arguments or large payload as argument.
- *Minimum requirement: **C++11*** :ok:
- *Recommended: **C++17*** :cool:

## Member functions
> short_opt:```string```, long_opt: ```string```
* *programName*() => ```const char*```
* *empty*() => ```bool```
* *has*(short_opt, long_opt) => ```bool```
* *exists*(short_opt, long_opt) ~ alias for has
* *find*(short_opt, long_opt) => ```iterator```
* *found*(iterator) => ```bool```
* *get*(iterator) => ```string```|```string_view```**(c++17)**
* *getString*(iterator) => ```string```
* *getList*(iterator,delimiter) => ```vector<string>```
* *getInt*(iterator) => ```int64_t```
* *getDouble*(iterator) => ```double```
* *getHex*(iterator) => ```int64_t```
## Member functions(C++17) :new:
> short_opt:```string```, long_opt: ```string```
* *get*(short_opt, long_opt) => ```optional< string_view >```
* *getString*(short_opt, long_opt) => ```optional< string >```
* *getList*(short_opt, long_opt) => ```optional< vector<string> >```
* *getInt*(short_opt, long_opt) => ```optional< int64_t >```
* *getDouble*(short_opt, long_opt) => ```optional< double >```
* *getHex*(short_opt, long_opt) => ```optional< int64_t >```

## Example for using has()
### Terminal 
    
    myprogram -v --disable 
    myprogram --verbose --disable
    
### Code

```c++
ArgumentParser argparser(argc, argv);
const bool verbose_flag = argparser.has("-v", "--verbose");
const bool disable_flag = argparser.has("", "--disable");//it has no short version
```
    
## Example for using find(), getInt()
### Terminal

    myprogram -V 58
    myprogram --volume 34
    myprogram --volume=67

### Code
```c++
ArgumentParser argparser(argc, argv);
auto sound_volume_option = argparser.find("-V", "--volume");
int64_t sound_volume = 100;//percent
if( argparser.found(sound_volume_option) )
{ 
    sound_volume = argparser.getInt(sound_volume_option); 
}
```

### Code(C++17)
> There is no need for using find() and then checking with found(). The same result can be achieved with getInt().
```c++
ArgumentParser argparser(argc, argv);
int64_t sound_volume = 100;//percent
if( auto sound_opt = argparser.getInt("-V", "--volume") )
{ 
    sound_volume = sound_opt.value();
}
```

## Example for getList()
### Terminal
    myprogram -l 1,two,3,four,5
    myprogram --list=a,b,c,d
    
### Code
```c++
ArgumentParser arguments(argc, argv);
ArgumentParser::string_list_t list;
auto list_option = arguments.find("-l", "--list");
if( arguments.found(list_option) )
{
    list = arguments.getList(list_option, ",");
}
```

### Code(C++17)
```c++
ArgumentParser arguments(argc, argv);
ArgumentParser::string_list_t list;
if( auto list_option = arguments.getList("-l", "--list", ",") )
{ 
    list = list_option.value(); 
}
```

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
               "\t-m, --message [MSG]   Expects some message\n"
               "\t-i, --integer [VALUE] Expects integer value\n"
               "\t-d, --double=[VALUE]  Expects double value\n"
               "\t-x, --hex [VALUE]     Expects hexa value\n"
               "\t-s                    Short version option\n"
               "\t    --long            Long version option\n"
               "\t-o, --optional        It is optional\n"
                "\t-h, --help            Show this help message\n\n"
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
    ArgumentParser::string_list_t list;
    
    #if __cplusplus == 201703L
    //C++17
        if( auto integer_option = arguments.getInt("-i", "--integer") )
        { integer_value = integer_option.value(); }

        if( auto double_option = arguments.getDouble("-d", "--double") )
        { double_value = double_option.value(); }

        if( auto hex_option = arguments.getInt("-x", "--hex") )
        { 
            hexadecimal = hex_option.value();
            hex_as_str = arguments.getString("-x", "--hex").value();
        }

        if( auto msg_option = arguments.get("-m", "--message") )
        { message = msg_option.value(); }

        if( auto list_option = arguments.getList("-l", "--list", ",") )
        { list = list_option.value(); }
    #else
    //C++11
        auto integer_option = arguments.find("-i", "--integer");
        if( arguments.found(integer_option) )
        {
            integer_value = arguments.getInt(integer_option);
        }

        auto double_option = arguments.find("-d", "--double");
        if( arguments.found(double_option) )
        {
            double_value = arguments.getDouble(double_option);
        }

        auto hex_option = arguments.find("-x", "--hex");
        if( arguments.found(hex_option) )
        {
            hex_as_str = arguments.get(hex_option);
            hexadecimal = arguments.getHex(hex_option);
        }

        auto msg_option = arguments.find("-m", "--message");
        if( arguments.found(msg_option) )
        {
            message = arguments.get(msg_option);
        }

        auto list_option = arguments.find("-l", "--list");
        if( arguments.found(list_option) )
        {
            list = arguments.getList(list_option, ",");
        }
    #endif
    
    printf("Read values:\n\tinteger(%ld), double(%f), hex(%s:%ld)\n"
           "\tflag(%s), short(%s), long(%s)\n"
           "\tmessage: '%s'\n"
          , integer_value
          , double_value
          , hex_as_str.c_str()
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
 
