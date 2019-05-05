#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <cstdlib>

#if __cplusplus < 201703L
#   pragma GCC error "ArgumentParser requires C++17 support!"
#endif

#include <string_view>

class ArgumentParser final
{
public:
    typedef std::vector<std::string_view> arguments_t;
    typedef arguments_t stringview_list_t;

    ArgumentParser(int argc, char** argv, const std::string& short_options = "") : args(), program_name(argv[0]), short_option_tips(short_options)
    {
        args.reserve(argc);
        for(int i = 0; i < argc; ++i) { args.emplace_back(argv[i]); }
    }

    ArgumentParser(const arguments_t& _args) : args(_args) 
    { 
        if( !args.empty() ) { program_name = std::string(args.front()); }
    }
    
    const char* programName() const { return program_name.c_str(); }

    bool empty() const  { return (args.size() < 2); }

    size_t size() const { return args.size(); }

    std::string_view front() const { return args.front(); }
    
    std::string_view back() const { return args.back(); }

    std::string_view back(size_t pos) const { return args[args.size() - 1- pos]; }

    std::string_view at(size_t pos) const { return args.at(pos); }

    std::string_view operator[](size_t pos) const { return args[pos]; }

    bool has(const std::string& short_opt, const std::string& long_opt) const
    { return find(short_opt, long_opt) != args.cend(); }

    std::optional<std::string_view> get(const std::string& short_opt, const std::string& long_opt) const
    {
        auto arg = find(short_opt, long_opt);
        return ( found(arg) ? std::make_optional<std::string_view>(get(arg, short_opt, long_opt)) : std::nullopt );
    }

    std::optional<std::string> getString(const std::string& short_opt, const std::string& long_opt) const 
    {
        if( auto opt = get(short_opt, long_opt) )
        { return std::string(opt.value()); } 
        else 
        { return std::nullopt; }
    }

    std::optional<int64_t> getInt(const std::string& short_opt, const std::string& long_opt, int base = 10) const
    {
        if( auto opt = get(short_opt, long_opt) )
        { return std::strtoll(opt.value().data(), nullptr, base); } 
        else
        { return std::nullopt; }
    }

    std::optional<double> getDouble(const std::string& short_opt, const std::string& long_opt) const
    {
        if( auto opt = get(short_opt, long_opt) )
        { return std::strtold(opt.value().data(), nullptr); } 
        else
        { return std::nullopt; }
    }

    std::optional<int64_t> getHex(const std::string& short_opt, const std::string& long_opt) const
    { return getInt(short_opt, long_opt, 16); }

    std::optional<stringview_list_t> getList(const std::string& short_opt, const std::string& long_opt, const std::string& delim) const
    {
        auto arg = find(short_opt, long_opt);
        return ( found(arg) ? std::make_optional<stringview_list_t>(getList(arg, delim, short_opt, long_opt)) : std::nullopt );
    }

private:
    arguments_t args;
    std::string program_name;
    std::string short_option_tips;

    arguments_t::const_iterator find(const std::string& short_opt, const std::string& long_opt) const
    {
        return std::find_if(args.cbegin(), args.cend(), [long_opt, short_opt, this](const arguments_t::value_type& option) -> bool 
        {
            if( (!long_opt.empty()) && (option.find(long_opt) != std::string::npos ) )
            { return true; }

            if( !short_opt.empty() && ( (option[0] == '-') && (option[1] != '-') ) )
            {
                size_t pos = option.find(short_opt[1]);
                if( pos == 1 ) 
                { return true; } 
                else if(pos != std::string::npos)
                {
                    if(short_option_tips.empty()) { return true; }
                    std::string tips_cpy = short_option_tips;
                    size_t i = 1;
                    for(; i < option.length(); ++i)
                    { 
                        pos = tips_cpy.find(option[i]);
                        if( pos == std::string::npos ) { break; }
                        else { tips_cpy.erase(pos); } 
                    }
                    return (i == option.length());
                }
            }
            return false;
        });
    }

    bool found(const arguments_t::const_iterator& arg_it) const
    { return arg_it != args.cend(); }

    std::string_view get(arguments_t::const_iterator& arg_it, const std::string& short_opt, const std::string& long_opt) const
    {
        if( arg_it != args.cend() )
        {
            const std::string_view& option = *arg_it;
            size_t pos = option.find('=');
            if( pos != std::string_view::npos )
            {
                return option.substr(pos+1);
            }
            else if( !short_opt.empty() && (option.length() > short_opt.length()) && (option.find(short_opt[1]) == 1) ) 
            {
                return option.substr(short_opt.length());
            }
            else if( !long_opt.empty() && (option.length() > long_opt.length()) && (option.find(long_opt) != std::string_view::npos) )
            {
                return option.substr(long_opt.length());
            }
            else {
                ++arg_it;
                if( arg_it != args.cend() )
                {
                    auto res = *arg_it;
                    --arg_it;
                    return res;
                }
            }
        }
        return "";
    }

    stringview_list_t getList(arguments_t::const_iterator& arg_it, const std::string& delim, const std::string& short_opt, const std::string& long_opt) const
    {
        stringview_list_t list;
        auto packed_value = get(arg_it, short_opt, long_opt);
        size_t pos = packed_value.find(delim);
        while( pos != std::string_view::npos )
        {
            if( pos > 0)
            { list.emplace_back(packed_value.substr(0, pos)); }
            packed_value = packed_value.substr(pos + 1);
            pos = packed_value.find(delim);
        }

        if( !packed_value.empty() )
        { list.emplace_back(packed_value); }

        return list;
    }
};

#endif
