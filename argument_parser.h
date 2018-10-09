#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

class ArgumentParser
{
    #if __cplusplus == 201703L
        typedef std::vector<std::string_view> arguments_t;
        typedef std::string_view string_value_t;
    #else
        typedef std::vector<std::string> arguments_t;
        typedef std::string string_value_t;
    #endif

    arguments_t args;
    std::string process_name;
public:
    ArgumentParser(int argc, char** argv)
    {
        args.reserve(argc);
        for(int i = 0; i < argc; ++i){ args.emplace_back(argv[i]); }
        process_name = std::string(args[0]);
    }
    
    bool empty()
    { return (args.size() < 2); }

    const std::string& processName()
    { return process_name; }

    arguments_t::const_iterator find(const std::string& short_opt, const std::string& long_opt)
    {
        return std::find_if(args.cbegin(), args.cend(), [long_opt, short_opt](const arguments_t::value_type& option) -> bool 
        {
            return ( ( (!long_opt.empty()) && (option.find(long_opt) != std::string::npos ) )
                || ( !short_opt.empty() && ((option[0] == '-') && (option[1] != '-') && (option.find(short_opt[1]) != std::string::npos)) ) );
        });    
    }

    bool has(const std::string& short_opt, const std::string& long_opt)
    { return find(short_opt, long_opt) != args.cend(); }

    bool found(const arguments_t::const_iterator& arg_it)
    { return arg_it != args.cend(); }

    std::string get(arguments_t::const_iterator& arg_it)
    {
        if( arg_it != args.cend() )
        {
            const std::string& option = std::string(*arg_it);
            size_t pos = 0;
            if( (pos = option.find('=')) != std::string::npos )
            {
                return option.substr(pos+1);
            } else {
                ++arg_it;
                if( arg_it != args.cend() )
                {
                    auto res = std::string(*arg_it);
                    --arg_it;
                    return res;
                }
            }
        }
        return "";
    }

    int64_t getInt(arguments_t::const_iterator& arg_it, int base = 10)
    {
        std::string value = get(arg_it);
        return ( ( !value.empty() ) ? (std::strtoll(value.c_str(), nullptr, base)) : (0) );
    }

    double getDouble(arguments_t::const_iterator& arg_it)
    {
        std::string value = get(arg_it);
        return ( ( !value.empty() ) ? (std::strtold(value.c_str(), nullptr)) : (0.0f) );
    }
    
    int64_t getHex(arguments_t::const_iterator& arg_it)
    { return getInt(arg_it, 16); }
};

#endif
