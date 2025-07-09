#include "cmd_line.h"

#include <expected>
#include <string_view>
#include <vector>

std::expected<std::vector<Argument>, std::string_view>
parseArguments(const std::vector<std::string_view>& args) {
    std::vector<Argument> ret {};

    for (int i = 0; i < args.size(); ++i) {
        Argument arg {};
        if (args[i][0] == '-') {
            arg.option = args[i];
        } else {
            return std::unexpected("There is an argument without the option.");
        }

        if (((i + 1) < args.size()) && (args[i + 1][0] != '-')) {
            arg.value = args[i + 1];
            i = i + 1;
        }

        ret.push_back(arg);
    }

    return ret;
}

std::expected<std::string_view, std::string_view>
searchArguments(std::vector<Argument> args, std::string_view option) {
    for (auto arg : args) {
        if (arg.option == option) {
            return arg.value;
        }
    }

    return std::unexpected("Option not found");
}

