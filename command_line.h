#ifndef _COMMAND_LINE_H_
#define _COMMAND_LINE_H_

#include <cstdio>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "argument_parser.h"

class CommandLine
{
public:
    static constexpr size_t DEFUALT_LINE_SIZE = 256;
    CommandLine(size_t line_size = DEFUALT_LINE_SIZE) 
        : line_buffer_size(line_size), command_map(), exit_command_name("quit") 
    {}
    /**
     * 
     */
    void registerCommand(const std::string& name, const std::function<int (const ArgumentParser&)>& function)
    { command_map.emplace(name, function); }
    /**
     * 
     */
    void registerExit(const std::string& name)
    { exit_command_name = name; }
    /**
     * 
     */
    std::vector<std::string> commandNames() const
    {
        std::vector<std::string> cmd_names;
        for(auto it = command_map.cbegin(); it != command_map.cend(); ++it ) { cmd_names.emplace_back(it->first); }
        cmd_names.emplace_back(exit_command_name);
        return cmd_names;
    }
    /**
     * 
     */
    int run() const
    {   
        int exit_code = 0;
        bool quit = false;
        std::unique_ptr<std::vector<char>> line_buffer(new std::vector<char>(line_buffer_size));
        while(!quit)
        {
            if( fgets(line_buffer->data(), line_buffer->size(), stdin) != nullptr )
            {
                std::string_view line(line_buffer->data());
                line.remove_suffix(1);
                auto args = string_view_split(line, " ");
                if( !args.empty() )
                {
                    std::string cmd_str(args.front());
                    if(exit_command_name == cmd_str) 
                    { quit = true; }
                    else 
                    {
                        const auto cmd_it = command_map.find(cmd_str);
                        if( (cmd_it != command_map.cend()) && cmd_it->second )
                        {
                            ArgumentParser parser(args);
                            exit_code = cmd_it->second(parser);
                            quit = exit_code != 0;
                        } else {
                            printf("Invalid command: %s\n", cmd_str.c_str());
                        }
                    }
                }
            } else { quit = true; }
        }
        return exit_code;
    }
protected:
    inline std::vector<std::string_view> string_view_split(const std::string_view& strv, const std::string& delim) const
    {
        std::vector<std::string_view> result;
        std::string_view strv_cpy(strv);
        size_t pos = strv_cpy.find(delim);
        while( pos != std::string_view::npos )
        {
            if( pos > 0)
            { result.emplace_back(strv_cpy.substr(0, pos)); }
            strv_cpy = strv_cpy.substr(pos + 1);
            pos = strv_cpy.find(delim);
        }
        
        if( !strv_cpy.empty() )
        { result.emplace_back(strv_cpy); }
        
        return result;
    }

    const size_t                                                      line_buffer_size;
    std::map<std::string, std::function<int (const ArgumentParser&)>> command_map;
    std::string                                                       exit_command_name;
};

#endif
