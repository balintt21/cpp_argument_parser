#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

namespace argument_parser
{
    std::vector<std::string> read(int argc, char** argv)
    {
        std::vector<std::string> arguments;
        arguments.reserve(argc);
        for(int i = 0; i < argc; ++i){ arguments.emplace_back(argv[i]); }
        return std::move(arguments);
    }
    
    bool empty(const std::vector<std::string>& args)
    {
        return (args.size() < 2);
    }

    const std::string& process_name(const std::vector<std::string>& args)
    {
        return args[0];
    }

    std::vector<std::string>::const_iterator find(const std::string& short_opt, const std::string& long_opt, const std::vector<std::string>& args)
    {
        return std::find_if(args.cbegin(), args.cend(), [long_opt, short_opt](const std::string& option) -> bool 
        {
            return ( (option.find(long_opt) != std::string::npos ) || (!short_opt.empty() && ((option[0] == '-') && (option[1] != '-') && (option.find(short_opt[1]) != std::string::npos))) );
        });    
    }

    bool has(const std::string& short_opt, const std::string& long_opt, const std::vector<std::string>& args)
    { 
        return find(short_opt, long_opt, args) != args.cend(); 
    }

    std::string get(std::vector<std::string>::const_iterator& arg_it, const std::vector<std::string>& args)
    {
        if( arg_it != args.cend() )
        {
            const std::string& option = (*arg_it);
            size_t pos = 0;
            if( (pos = option.find('=')) != std::string::npos )
            {
                return option.substr(pos+1);
            } else {
                ++arg_it;
                if( arg_it != args.cend() )
                {
                    return (*arg_it);
                }
            }
        }
        return "";
    }

    int64_t getInt(std::vector<std::string>::const_iterator& arg_it, const std::vector<std::string>& args, int base = 10)
    {
        std::string value = get(arg_it, args);
        if( !value.empty() )
        {
            return std::strtoll(value.c_str(), nullptr, base);
        }
        return 0;
    }

    int64_t getDouble(std::vector<std::string>::const_iterator& arg_it, const std::vector<std::string>& args)
    {
        std::string value = get(arg_it, args);
        if( !value.empty() )
        {
            return std::strtold(value.c_str(), nullptr);
        }
        return 0.0f;
    }
    
    int64_t getHex(std::vector<std::string>::const_iterator& arg_it, const std::vector<std::string>& args)
    {
        return getInt(arg_it, args, 16);
    }
}

#endif
