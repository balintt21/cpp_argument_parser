#!/usr/bin/env python

import sys, os, json, base64

option_map_h_code_base64 = "I2luY2x1ZGUgPHN0ZGludC5oPgojaW5jbHVkZSA8c3RyaW5nPgojaW5jbHVkZSA8c3RyaW5nX3ZpZXc+CiNpbmNsdWRlIDxvcHRpb25hbD4KI2luY2x1ZGUgPHZlY3Rvcj4KI2luY2x1ZGUgPHVub3JkZXJlZF9tYXA+CiNpbmNsdWRlIDxtZW1vcnk+CgpuYW1lc3BhY2Ugb3B0aW9uX21hcAp7CiAgICBzdHJ1Y3QgVmFsdWUKICAgIHsKICAgICAgICBlbnVtIFR5cGUgeyBOVU1CRVIsIFNUUklORywgTElTVCB9OwogICAgICAgIHVuaW9uIE51bWJlcgogICAgICAgIHsKICAgICAgICAgICAgaW50NjRfdCB2YWx1ZV9pbnQ7CiAgICAgICAgICAgIGRvdWJsZSAgdmFsdWVfZG91YmxlOwogICAgICAgICAgICBOdW1iZXIoKSA6IHZhbHVlX2ludCgwKSB7fQogICAgICAgICAgICBOdW1iZXIoaW50NjRfdCB2YWx1ZSkgOiB2YWx1ZV9pbnQodmFsdWUpIHt9CiAgICAgICAgICAgIE51bWJlcihkb3VibGUgdmFsdWUpIDogdmFsdWVfZG91YmxlKHZhbHVlKSB7fQogICAgICAgIH07CgogICAgICAgIFR5cGUgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIHR5cGU7CiAgICAgICAgTnVtYmVyICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgbnVtYmVyOwogICAgICAgIHN0ZDo6c3RyaW5nX3ZpZXcgICAgICAgICAgICAgICAgICAgIHN0cmluZzsKICAgICAgICBzdGQ6OnVuaXF1ZV9wdHI8c3RkOjp2ZWN0b3I8VmFsdWU+PiBsaXN0OwoKICAgICAgICBWYWx1ZSgpIDogdHlwZShWYWx1ZTo6TlVNQkVSKSwgbnVtYmVyKCksIHN0cmluZygpLCBsaXN0KG51bGxwdHIpIHt9CiAgICAgICAgVmFsdWUoaW50NjRfdCB2YWx1ZSkgOiB0eXBlKFZhbHVlOjpOVU1CRVIpLCBudW1iZXIodmFsdWUpLCBzdHJpbmcoKSwgbGlzdChudWxscHRyKSB7fQogICAgICAgIFZhbHVlKGRvdWJsZSB2YWx1ZSkgOiB0eXBlKFZhbHVlOjpOVU1CRVIpLCBudW1iZXIodmFsdWUpLCBzdHJpbmcoKSwgbGlzdChudWxscHRyKSB7fQogICAgICAgIFZhbHVlKGNvbnN0IHN0ZDo6c3RyaW5nJiB2YWx1ZSkgOiB0eXBlKFZhbHVlOjpTVFJJTkcpLCBudW1iZXIoKSwgc3RyaW5nKHZhbHVlKSwgbGlzdChudWxscHRyKSB7fQogICAgICAgIFZhbHVlKGNvbnN0IHN0ZDo6dmVjdG9yPHN0ZDo6c3RyaW5nX3ZpZXc+JiB2YWx1ZSkgOiB0eXBlKFZhbHVlOjpOVU1CRVIpLCBudW1iZXIoKSwgc3RyaW5nKCksIGxpc3QobnVsbHB0cikgCiAgICAgICAgeyAKICAgICAgICAgICAgbGlzdC5yZXNldChuZXcgc3RkOjp2ZWN0b3I8VmFsdWU+KCkpOwogICAgICAgICAgICBpZighdmFsdWUuZW1wdHkoKSkKICAgICAgICAgICAgewogICAgICAgICAgICAgICAgZm9yKGNvbnN0IGF1dG8mIHN0cl92aWV3IDogdmFsdWUpCiAgICAgICAgICAgICAgICB7IGxpc3QtPmVtcGxhY2VfYmFjayhzdHJfdmlldyk7IH0KICAgICAgICAgICAgfQogICAgICAgIH0KICAgICAgICBpbmxpbmUgYm9vbCBpc051bWJlcigpIGNvbnN0IHsgcmV0dXJuIHR5cGUgPT0gVmFsdWU6Ok5VTUJFUjsgfQogICAgICAgIGlubGluZSBib29sIGlzU3RyaW5nKCkgY29uc3QgeyByZXR1cm4gdHlwZSA9PSBWYWx1ZTo6U1RSSU5HOyB9CiAgICAgICAgaW5saW5lIGJvb2wgaXNMaXN0KCkgICBjb25zdCB7IHJldHVybiB0eXBlID09IFZhbHVlOjpMSVNUOyB9CiAgICAgICAgaW5saW5lIGludDY0X3QgYXNJbnQoKSAgICAgICAgICAgICAgIGNvbnN0IHsgcmV0dXJuIG51bWJlci52YWx1ZV9pbnQ7IH0KICAgICAgICBpbmxpbmUgaW50NjRfdCBhc0RvdWJsZSgpICAgICAgICAgICAgY29uc3QgeyByZXR1cm4gbnVtYmVyLnZhbHVlX2RvdWJsZTsgfQogICAgICAgIGlubGluZSBjb25zdCBzdGQ6OnN0cmluZyYgYXNTdHJpbmcoKSBjb25zdCB7IHJldHVybiBzdGQ6OnN0cmluZyhzdHJpbmcpOyB9CiAgICAgICAgaW5saW5lIGNvbnN0IHN0ZDo6c3RyaW5nX3ZpZXcmIGFzU3RyaW5nVmlldygpIGNvbnN0IHsgcmV0dXJuIHN0cmluZzsgfQogICAgICAgIGlubGluZSBjb25zdCBzdGQ6OnZlY3RvcjxWYWx1ZT4mICBhc0xpc3QoKSBjb25zdCB7IHJldHVybiAoIGxpc3QgPyAoKmxpc3QpIDogKHN0ZDo6dmVjdG9yPFZhbHVlPigpKSApOyB9CiAgICB9OwoKICAgIHR5cGVkZWYgc3RkOjpvcHRpb25hbDxWYWx1ZT4gb3B0aW9uX3ZhbHVlX3Q7Cn0KCnR5cGVkZWYgc3RkOjp1bm9yZGVyZWRfbWFwPHN0ZDo6c3RyaW5nLCBvcHRpb25fbWFwOjpvcHRpb25fdmFsdWVfdD4gb3B0aW9uX21hcF90Owo="

def generateHelpOptionLine(entry):
    option_line = f"\\t-{entry['short']}, " if "short" in entry else "\\t    "
    option_line += f"--{entry['long']} " if "long" in entry else " "
    option_line += f"[{entry['type'].upper()}] "
    option_line += f"{entry['help']}\\n" if "help" in entry else "\\n"
    return option_line

def generateCode(data):
    code = "#ifndef GENERATED_OPTION_PARSER_H_\n#define GENERATED_OPTION_PARSER_H_\n\n#include \"argument_parser.h\"\n"
    code += base64.b64decode(option_map_h_code_base64).decode("utf-8")
    get_options_fun = "\nstd::string getOptionsHelp()\n{\n\treturn \"Options\\n"
    parser_code = "option_map_t parseOptions(const ArgumentParser& args)\n{\n\toption_map_t opt_map;\n"
    for entry in data:
        has_short = "short" in entry
        has_long = "long" in entry
        if "name" in entry and "type" in entry and (has_short or has_long):
            get_options_fun += generateHelpOptionLine(entry)
            short_opt = f"\"-{entry['short']}\"" if has_short else "\"\""
            long_opt = f"\"--{entry['long']}\"" if has_long else "\"\"" 
            type_fun = ""
            delim_param = ""
            if entry['type'] == "int":
                type_fun = "getInt"
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
                parser_code += f"\tif(auto opt = args.{type_fun}({short_opt}, {long_opt}{delim_param})) {{ opt_map[\"{entry['name']}\"] = option_map::Value(opt.value()); }}\n"
                
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
