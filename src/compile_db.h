#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

struct CompilationEntry {
    std::string_view directory;
    std::vector<std::string> arguments;
    std::string_view file;
};

inline void to_json(nlohmann::json& j, const CompilationEntry& c) {
    j = nlohmann::json{
        {"directory", c.directory},
        {"arguments", c.arguments},
        {"file", c.file}
    };
}

inline void from_json(const nlohmann::json& j, CompilationEntry& c) {
    j.at("directory").get_to(c.directory);
    j.at("arguments").get_to(c.arguments);
    j.at("file").get_to(c.file);
}

