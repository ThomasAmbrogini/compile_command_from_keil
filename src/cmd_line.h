#pragma once

#include <expected>
#include <string_view>
#include <vector>

struct Argument {
    std::string_view option;
    std::string_view value;
};

std::expected<std::vector<Argument>, std::string_view>
parseArguments(const std::vector<std::string_view>& args);

std::expected<std::string_view, std::string_view>
searchArguments(std::vector<Argument> args, std::string_view option);

