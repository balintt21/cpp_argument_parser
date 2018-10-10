#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <cstdlib>

class ArgumentParser final
{
public:
    #if __cplusplus == 201703L
        typedef std::string_view string_value_t;
    #else
        typedef std::string string_value_t;
    #endif
    typedef std::vector<string_value_t> arguments_t;
    typedef arguments_t string_list_t;
private:
    arguments_t args;
    const char* process_name;
public:
    ArgumentParser(int argc, char** argv) : args(), process_name(argv[0])
    {
        args.reserve(argc);
        for(int i = 0; i < argc; ++i){ args.emplace_back(argv[i]); }
    }
    
    bool empty()
    { return (args.size() < 2); }

    const char* processName() const
    { return process_name; }

    arguments_t::const_iterator find(const std::string& short_opt, const std::string& long_opt) const
    {
        return std::find_if(args.cbegin(), args.cend(), [long_opt, short_opt](const arguments_t::value_type& option) -> bool 
        {
            return ( ( (!long_opt.empty()) && (option.find(long_opt) != std::string::npos ) )
                || ( !short_opt.empty() && ((option[0] == '-') && (option[1] != '-') && (option.find(short_opt[1]) != std::string::npos)) ) );
        });
    }

    bool has(const std::string& short_opt, const std::string& long_opt) const
    { return find(short_opt, long_opt) != args.cend(); }

    bool exists(const std::string& short_opt, const std::string& long_opt) const 
    { return has(short_opt, long_opt); }

    bool found(const arguments_t::const_iterator& arg_it) const
    { return arg_it != args.cend(); }

    string_value_t get(arguments_t::const_iterator& arg_it) const
    {
        if( arg_it != args.cend() )
        {
            const string_value_t& option = *arg_it;
            size_t pos = option.find('=');
            if( pos != string_value_t::npos )
            {
                return option.substr(pos+1);
            } else {
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

    string_list_t getList(arguments_t::const_iterator& arg_it, const std::string& delim) const
    {
        string_list_t list;
        auto packed_value = get(arg_it);
        size_t pos = packed_value.find(delim);
        while( pos != string_value_t::npos )
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

    std::string getString(arguments_t::const_iterator& arg_it) const
    {
        return std::string(get(arg_it));
    }

    int64_t getInt(arguments_t::const_iterator& arg_it, int base = 10) const
    {
        std::string value(get(arg_it));
        return ( ( !value.empty() ) ? (std::strtoll(value.c_str(), nullptr, base)) : (0) );
    }

    double getDouble(arguments_t::const_iterator& arg_it) const
    {
        std::string value(get(arg_it));
        return ( ( !value.empty() ) ? (std::strtold(value.c_str(), nullptr)) : (0.0f) );
    }
    
    int64_t getHex(arguments_t::const_iterator& arg_it) const
    { return getInt(arg_it, 16); }

    #if __cplusplus == 201703L

    std::optional<string_value_t> get(const std::string& short_opt, const std::string& long_opt) const
    {
        auto arg = find(short_opt, long_opt);
        return ( found(arg) ? std::make_optional<string_value_t>(get(arg)) : std::nullopt );
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
        { 
            std::string value(opt.value());
            return ( ( !value.empty() ) ? (std::strtoll(value.c_str(), nullptr, base)) : (0) );
        } 
        else
        { return std::nullopt; }
    }

    std::optional<double> getDouble(const std::string& short_opt, const std::string& long_opt) const
    {
        if( auto opt = get(short_opt, long_opt) )
        { 
            std::string value(opt.value());
            return ( ( !value.empty() ) ? (std::strtold(value.c_str(), nullptr)) : (0) );
        } 
        else
        { return std::nullopt; }
    }

    std::optional<int64_t> getHex(const std::string& short_opt, const std::string& long_opt) const
    { return getInt(short_opt, long_opt, 16); }

    std::optional<string_list_t> getList(const std::string& short_opt, const std::string& long_opt, const std::string& delim) const
    {
        auto arg = find(short_opt, long_opt);
        return ( found(arg) ? std::make_optional<string_list_t>(getList(arg, delim)) : std::nullopt );
    }

    #endif

};

#endif
