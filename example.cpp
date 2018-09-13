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
               "\t-h, --help            Show this help message\n"
              , arguments.processName().c_str() );
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

    return 0;
}