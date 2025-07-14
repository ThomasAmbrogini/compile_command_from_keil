#include <cstdio>
#include <expected>
#include <filesystem>
#include <fmt/base.h>
#include <fstream>
#include <ranges>
#include <regex>
#include <string_view>
#include <thread>
#include <vector>
#include <unordered_map>

#include "tinyxml2.h"
#include <nlohmann/json.hpp>

#include "cmd_line.h"
#include "compile_db.h"
#include "xml_analysis.h"

namespace fs = std::filesystem;

tinyxml2::XMLElement* findTarget(tinyxml2::XMLElement* element, std::string_view search_target_name) {
    constexpr std::string_view target_element_name {"Target"};
    tinyxml2::XMLElement* target_el {searchDF(element, target_element_name, 1)};

    while(target_el) {
        std::string_view target_name {target_el->FirstChildElement("TargetName")->FirstChild()->Value()};
        if (target_name == search_target_name) {
            break;
        }
        target_el = target_el->NextSiblingElement();
    }

    return target_el;
}

std::string_view getDeviceName(std::string_view device_element) {
    static const std::unordered_map<std::string_view, std::string_view>
        device_elements = { {"ATSAME70Q21", "__SAME70Q21__"},
                            {"STM32F051T8Yx", "STM32F051x8"},
                            {"STM32F373VCTx", "STM32F373VC"} };


    auto it = device_elements.find(device_element);
    if (it != device_elements.end()) {
        return it->second;
    }

    return {};
}

void usage() {
    std::puts("./executable [-hd] -f keil_proj -t target\n\
                \n\
               -h for the current help window.\n\
               -f keil project path.\n\
               -t target of the keil project.\n\
               -d specifies the directory in which the output has to be created.\n\
               ");
}

int main(int argc, char* argv[]) {
    using namespace tinyxml2;
    using json = nlohmann::json;

    constexpr static const char* include_path_to_add[] = {
        "C:\\Users\\TAmbrogini\\sls\\sls\\references\\SLS_Application\\projects\\uVision\\RTE\\Device\\ATSAME70Q21",
        "C:\\Keil_v5\\ARM\\PACK\\ARM\\CMSIS\\4.5.0\\CMSIS\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\MDK-Middleware\\7.0.0\\FileSystem\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\MDK-Middleware\\7.0.0\\Network\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\SAM-ESV7_SFP\\2.2.0\\Libraries\\libchip_samv7",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\SAM-ESV7_SFP\\2.2.0\\Libraries\\libchip_samv7\\include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\SAM-E_DFP\\2.1.3\\include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\STM32F3xx_DFP\\2.1.0\\Drivers\\CMSIS\\Device\\ST\\STM32F3xx\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\ARM\\CMSIS\\4.5.0\\CMSIS\\RTOS\\RTX\\INC",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\STM32F0xx_DFP\\2.1.1\\Drivers\\CMSIS\\Device\\ST\\STM32F0xx\\Include"
    };

    constexpr static const char* defines_to_add[] = {
        "_RTE_",
        "__UVISION_VERSION=\"517\"",
        "__CC_ARM",
        "__MICROLIB"
    };

    constexpr static const char* additional_defines[] = {
        "--c99",
        "-c",
        "--cpu",
        "Cortex-M7.fp.dp",
        "-g",
        "-O0",
        "--apcs=interwork",
        "--split_sections",
        "--signed_chars"
    };

    std::vector<std::string_view> args {argv + 1, argv + argc};

    auto ret { parseArguments(args) };
    if (!ret) {
        std::puts(ret.error().data());
        usage();
        return 0;
    }
    std::vector<Argument> arguments { ret.value() };

    auto help_window_expected { searchArguments(arguments, "-h") };
    if (help_window_expected) {
        usage();
        return 0;
    }

    auto keil_filename_expected { searchArguments(arguments, "-f") };
    if (!keil_filename_expected) {
        std::puts("Missing uvprojx path! Pass it with -f option.");
        usage();
        return 0;
    }

    std::string_view keil_project_filename { keil_filename_expected.value() };
    fmt::print("Keil file name: {}\n", keil_project_filename);

    fs::path keil_project_file { keil_project_filename };
    fs::path keil_project_file_abs { fs::absolute(keil_project_file).string() };

    XMLDocument doc;
    XMLError res_load = doc.LoadFile(keil_project_file_abs.string().c_str());
    if (res_load != 0) {
        fmt::print("Error loading the XML file: {}\n", keil_project_file_abs.string());
        usage();
        return 0;
    }

    bool found {false};
    XMLElement* root = doc.RootElement();
    const char* include_path_value {};
    const char* defines_value {};

    auto target_expected { searchArguments(arguments, "-t") };
    if (!target_expected) {
        std::puts("Missing target! Pass it with -t option.");
        usage();
        return 0;
    }
    const std::string target_name { target_expected.value() };
    XMLElement* target_element = findTarget(root, target_name);
    if(!target_element) {
        std::puts("The target was not found!");
        usage();
        return 1;
    }
    fmt::print("Target: {}\n", target_name);

    std::string include_string { std::string(getElementValue(target_element, "IncludePath")) };
    std::string defines_string { std::string(getElementValue(target_element, "Define")) };

    const std::regex re(R"([;]+)");
    std::sregex_token_iterator it{ include_string.begin(),
                                   include_string.end(), re, -1 };

    std::vector<std::string> tokenized_include { it, {} };
    tokenized_include.erase(
        std::remove_if(tokenized_include.begin(),
                            tokenized_include.end(),
                       [](std::string const& s) {
                           return s.size() == 0;
                       }),
        tokenized_include.end());

    for (int i = 0; i < sizeof(include_path_to_add)/sizeof(char*); ++i) {
        tokenized_include.push_back(include_path_to_add[i]);
    }

    //TODO: is there a way to not create another string everytime?
    for (int i = 0; i < tokenized_include.size(); ++i) {
        tokenized_include[i] = "-I" + tokenized_include[i];
    }

    std::vector<std::string> tokenized{ "armcc.exe" };
    tokenized.insert(tokenized.end(), tokenized_include.begin(), tokenized_include.end());

    const std::regex re_define(R"([,]+)");
    std::sregex_token_iterator it_define {defines_string.begin(), defines_string.end(),
        re_define, -1};
    std::vector<std::string> tokenized_defines{ it_define, {} };

    tokenized_defines.erase(
        std::remove_if(tokenized_defines.begin(),
                            tokenized_defines.end(),
                       [](std::string const& s) {
                           return s.size() == 0;
                       }),
        tokenized_defines.end());

    for (int i = 0; i < sizeof(defines_to_add)/sizeof(char*); ++i) {
        tokenized_defines.push_back(defines_to_add[i]);
    }

    XMLElement* device_element { searchDF(root, "Device", 1) };
    if (device_element) {
        const char* device_name = device_element->FirstChild()->Value();
        std::string_view device_define { getDeviceName(device_name) };
        if (device_define.size() != 0) {
            tokenized_defines.push_back(device_define.data());
        }
    }

    std::ranges::for_each(tokenized_defines, [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
            return !std::isspace(ch);
        }));

        /* Remove surrounding single quotes, if any. */
        if (!s.empty() && s.front() == '\'') s.erase(0, 1);
        if (!s.empty() && s.back() == '\'') s.pop_back();
    });

    //TODO: is there a way to not create another string everytime?
    for (int i = 0; i < tokenized_defines.size(); ++i) {
        tokenized_defines[i] = "-D" + tokenized_defines[i];
    }

    for (int i = 0; i < sizeof(additional_defines)/sizeof(char*); ++i) {
        tokenized_defines.push_back(additional_defines[i]);
    }

    tokenized.insert( tokenized.end(), tokenized_defines.begin(), tokenized_defines.end() );

    constexpr std::string_view filepath_tag {"FilePath"};
    std::vector<std::string_view> filepaths = getAllValues(target_element, filepath_tag);

    std::vector<CompilationEntry> entries {};
    std::string uvision_dir { "projects/uVision" };
    for (auto filepath : filepaths) {
        entries.push_back(CompilationEntry {
                .directory = uvision_dir,
                .arguments = tokenized,
                .file = filepath
                });
    }

    json j(entries);
    std::string compile_commands_path_str = "compile_commands.json";
    auto output_directory { searchArguments(arguments, "-d") };
    if (output_directory) {
        compile_commands_path_str = std::string(output_directory.value()) + "/" + compile_commands_path_str;
        fs::path compile_commands_path(compile_commands_path_str);
        fs::create_directories(compile_commands_path.parent_path());
    }

    std::ofstream o(compile_commands_path_str);
    o << std::setw(4) << j << std::endl;

    fmt::print("The compile_commands.json file has been created in: {}\n", fs::absolute(compile_commands_path_str).string());
    o.close();
}

