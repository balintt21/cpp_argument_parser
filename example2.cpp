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
            printf("\t%s\n", cmd_name.c_str());
        }
        return 0;
    });

    return cmd.run();
}