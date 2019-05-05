#!/usr/bin/env python

import sys, os, json, base64

option_map_h_code_base64 = "I2luY2x1ZGUgPGFzc2VydC5oPgojaW5jbHVkZSA8c3RkaW50Lmg+CiNpbmNsdWRlIDxzdHJpbmc+CiNpbmNsdWRlIDxzdHJpbmdfdmlldz4KI2luY2x1ZGUgPG9wdGlvbmFsPgojaW5jbHVkZSA8dmVjdG9yPgojaW5jbHVkZSA8dW5vcmRlcmVkX21hcD4KI2luY2x1ZGUgPG1lbW9yeT4KCm5hbWVzcGFjZSBvcHRpb25fbWFwCnsKICAgIHN0cnVjdCBWYWx1ZQogICAgewogICAgICAgIGVudW0gVHlwZSB7IE5VTUJFUiwgU1RSSU5HLCBMSVNUIH07CiAgICAgICAgdW5pb24gTnVtYmVyCiAgICAgICAgewogICAgICAgICAgICBpbnQ2NF90IHZhbHVlX2ludDsKICAgICAgICAgICAgZG91YmxlICB2YWx1ZV9kb3VibGU7CiAgICAgICAgICAgIE51bWJlcigpIDogdmFsdWVfaW50KDApIHt9CiAgICAgICAgICAgIE51bWJlcihpbnQ2NF90IHZhbHVlKSA6IHZhbHVlX2ludCh2YWx1ZSkge30KICAgICAgICAgICAgTnVtYmVyKGRvdWJsZSB2YWx1ZSkgOiB2YWx1ZV9kb3VibGUodmFsdWUpIHt9CiAgICAgICAgfTsKCiAgICAgICAgVHlwZSAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgdHlwZTsKICAgICAgICBOdW1iZXIgICAgICAgICAgICAgICAgICAgICAgICAgICAgICBudW1iZXI7CiAgICAgICAgc3RkOjpzdHJpbmcgICAgICAgICAgICAgICAgICAgICAgICAgc3RyaW5nOwogICAgICAgIHN0ZDo6c2hhcmVkX3B0cjxzdGQ6OnZlY3RvcjxWYWx1ZT4+IGxpc3Q7CgogICAgICAgIFZhbHVlKCkgOiB0eXBlKFZhbHVlOjpOVU1CRVIpLCBudW1iZXIoKSwgc3RyaW5nKCksIGxpc3QobnVsbHB0cikge30KICAgICAgICBWYWx1ZShpbnQ2NF90IHZhbHVlKSA6IHR5cGUoVmFsdWU6Ok5VTUJFUiksIG51bWJlcih2YWx1ZSksIHN0cmluZygpLCBsaXN0KG51bGxwdHIpIHt9CiAgICAgICAgVmFsdWUoZG91YmxlIHZhbHVlKSA6IHR5cGUoVmFsdWU6Ok5VTUJFUiksIG51bWJlcih2YWx1ZSksIHN0cmluZygpLCBsaXN0KG51bGxwdHIpIHt9CiAgICAgICAgVmFsdWUoY29uc3Qgc3RkOjpzdHJpbmcmIHZhbHVlKSA6IHR5cGUoVmFsdWU6OlNUUklORyksIG51bWJlcigpLCBzdHJpbmcodmFsdWUpLCBsaXN0KG51bGxwdHIpIHt9CiAgICAgICAgVmFsdWUoY29uc3Qgc3RkOjp2ZWN0b3I8c3RkOjpzdHJpbmdfdmlldz4mIHZhbHVlKSA6IHR5cGUoVmFsdWU6Ok5VTUJFUiksIG51bWJlcigpLCBzdHJpbmcoKSwgbGlzdChudWxscHRyKSAKICAgICAgICB7IAogICAgICAgICAgICBsaXN0LnJlc2V0KG5ldyBzdGQ6OnZlY3RvcjxWYWx1ZT4oKSk7CiAgICAgICAgICAgIGlmKCF2YWx1ZS5lbXB0eSgpKQogICAgICAgICAgICB7CiAgICAgICAgICAgICAgICBmb3IoY29uc3QgYXV0byYgc3RyX3ZpZXcgOiB2YWx1ZSkKICAgICAgICAgICAgICAgIHsgbGlzdC0+ZW1wbGFjZV9iYWNrKHN0ZDo6c3RyaW5nKHN0cl92aWV3KSk7IH0KICAgICAgICAgICAgfQogICAgICAgIH0KICAgICAgICBpbmxpbmUgYm9vbCBpc051bWJlcigpIGNvbnN0IHsgcmV0dXJuIHR5cGUgPT0gVmFsdWU6Ok5VTUJFUjsgfQogICAgICAgIGlubGluZSBib29sIGlzU3RyaW5nKCkgY29uc3QgeyByZXR1cm4gdHlwZSA9PSBWYWx1ZTo6U1RSSU5HOyB9CiAgICAgICAgaW5saW5lIGJvb2wgaXNMaXN0KCkgICBjb25zdCB7IHJldHVybiB0eXBlID09IFZhbHVlOjpMSVNUOyB9CiAgICAgICAgaW5saW5lIGludDY0X3QgYXNJbnQoKSAgICAgICAgICAgICAgIGNvbnN0IHsgcmV0dXJuIG51bWJlci52YWx1ZV9pbnQ7IH0KICAgICAgICBpbmxpbmUgZG91YmxlIGFzRG91YmxlKCkgICAgICAgICAgICAgY29uc3QgeyByZXR1cm4gbnVtYmVyLnZhbHVlX2RvdWJsZTsgfQogICAgICAgIGlubGluZSBjb25zdCBzdGQ6OnN0cmluZyYgYXNTdHJpbmcoKSBjb25zdCB7IHJldHVybiBzdHJpbmc7IH0KICAgICAgICBpbmxpbmUgY29uc3Qgc3RkOjp2ZWN0b3I8VmFsdWU+JiAgYXNMaXN0KCkgY29uc3QgeyBhc3NlcnQobGlzdCAhPSBudWxscHRyKTsgcmV0dXJuICpsaXN0OyB9CiAgICB9OwoKICAgIHR5cGVkZWYgc3RkOjpvcHRpb25hbDxWYWx1ZT4gb3B0aW9uX3ZhbHVlX3Q7Cn0KCnR5cGVkZWYgc3RkOjp1bm9yZGVyZWRfbWFwPHN0ZDo6c3RyaW5nLCBvcHRpb25fbWFwOjpvcHRpb25fdmFsdWVfdD4gb3B0aW9uX21hcF90Owo="

def generateHelpOptionLine(entry):
    option_line = f"\\t-{entry['short']}, " if "short" in entry else "\\t    "
    option_line += f"--{entry['long']} " if "long" in entry else " "
    option_line += f"[{entry['type'].upper()}] " if entry['type'] != "bool" else ""
    option_line += f"{entry['help']}\\n" if "help" in entry else "\\n"
    return option_line

def generateCode(data):
    code = "#ifndef GENERATED_OPTION_PARSER_H_\n#define GENERATED_OPTION_PARSER_H_\n\n#include \"argument_parser.h\"\n"
    code += base64.b64decode(option_map_h_code_base64).decode("utf-8")
    get_options_fun = "\nstd::string getOptionsHelp()\n{\n\treturn \"Options\\n"
    parser_code = "option_map_t parseOptions(ArgumentParser& args)\n{\n\toption_map_t opt_map;\n"
    short_tips = ""
    for entry in data:
        if "short" in entry:
            short_tips += entry['short']
    if short_tips:
        parser_code += f"\targs.setShortOptionTips(\"{short_tips}\");\n"
    for entry in data:
        has_short = "short" in entry
        has_long = "long" in entry
        if "name" in entry and "type" in entry and (has_short or has_long):
            get_options_fun += generateHelpOptionLine(entry)
            short_opt = f"\"-{entry['short']}\"" if has_short else "\"\""
            long_opt = f"\"--{entry['long']}\"" if has_long else "\"\"" 
            type_fun = ""
            delim_param = ""
            base = ""
            if entry['type'] == "int":
                type_fun = "getInt"
                base = f", {entry['base']}" if "base" in entry else ", 10"
            elif entry['type'] == "double":
                type_fun = "getDouble"
            elif entry['type'] == "string":
                type_fun = "getString"
            elif entry['type'] == "list":
                type_fun = "getList"
                delim_param = f", \"{entry['delim']}\"" if "delim" in entry else ", \",\""
            parser_code += f"\topt_map[\"{entry['name']}\"] = std::nullopt;\n"
            if not type_fun:
                parser_code += f"\tif(args.has({short_opt}, {long_opt})) {{ opt_map[\"{entry['name']}\"] = option_map::Value(1L); }}\n"
            else:
                parser_code += f"\tif(auto opt = args.{type_fun}({short_opt}, {long_opt}{delim_param}{base})) {{ opt_map[\"{entry['name']}\"] = option_map::Value(opt.value()); }}\n"
                
    get_options_fun += "\";\n}\n\n"
    parser_code += "\n\treturn opt_map;\n }\n"
    code += get_options_fun
    code += parser_code
    code += "\n#endif\n"
    return code

def interpretJson(file_path):
    code = ""
    try:
        file_size = os.path.getsize(file_path)
        with open(file_path, "r") as file:
            json_data = json.loads(file.read(file_size))
            code = generateCode(json_data)
    except OSError as e:
        print(e.strerror)
    except Exception as e:
        print(f"An exception has occured: \"{ str(e) }\"")
    return code

def main():
    if (len(sys.argv) < 2):
        print("Usage: {0} <json_file_path> [output_file_path]\n".format(sys.argv[0]))
    else:
        code = interpretJson(sys.argv[1])
        if len(sys.argv) < 3:
            print(code)
        else:
            with open(sys.argv[2], "w") as file:
                file.write(code)


main()
