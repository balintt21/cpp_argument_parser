#include <stdio.h>
#include "option_parser.h"

int main(int argc, char** argv)
{
    ArgumentParser args(argc, argv);
    auto options = parseOptions(args);
    if(args.empty() || options["Help"])
    {
        std::string options_str = getOptionsHelp();
        printf("Usage: %s [OPTIONS]\n%s", args.programName(), options_str.c_str());
        return 0;
    }

    printf("Read values:\n"
           "\tinteger(%ld)\n"
           "\tdouble(%f)\n"
           "\thex(0x%lx)\n"
           "\tflag(%s)\n"
           "\tshort(%s)\n"
           "\tlong(%s)\n"
           "\tmessage: '%s'\n"
          , options["Integer"] ? options["Integer"].value().asInt() : 0
          , options["Double"] ? options["Double"].value().asDouble() : 0
          , options["Hex"] ? options["Hex"].value().asInt() : 0
          , options["Optional"] ? "true" : "false"
          , options["Short"] ? "true" : "false"
          , options["Long"] ? "true" : "false"
          , options["Message"] ? options["Message"].value().asString().c_str() : "");
    puts("List:");
    if(options["List"])
    {
        for(const auto& element : options["List"].value().asList() )
        {
            printf("\t%s\n", element.asString().c_str());
        }
    }

    return 0;
}