#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <string_view>
#include <thread>
#include <vector>

#include "tinyxml2.h"
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

//TODO: this can return an expected.
tinyxml2::XMLElement* searchDF(tinyxml2::XMLElement* element, std::string_view find_el_name, int num_occurrence) {
    using namespace tinyxml2;

    XMLElement* ret {};
    int count {};
    std::vector<XMLElement*> frontier;
    frontier.push_back(element);

    while(frontier.size() != 0) {
        element = frontier.back();
        frontier.pop_back();
        if (element->Name() == find_el_name) {
            ++count;
            if (count == num_occurrence) {
                ret = element;
                break;
            }
        }

        if (element->LastChildElement()) {
            element = element->LastChildElement();
            frontier.push_back(element);
            while(element->PreviousSiblingElement()) {
                element = element->PreviousSiblingElement();
                frontier.push_back(element);
            }
        }
    }

    return ret;
}

tinyxml2::XMLElement* searchDFRecursive(tinyxml2::XMLElement* element, std::string_view find_el_name, int& num_occurrence) {
    using namespace tinyxml2;

    if (!element) {
        return nullptr;
    }

    if (element->Name() == find_el_name) {
        --num_occurrence;
        if (num_occurrence == 0) {
            return element;
        }
    }

    if (element->FirstChildElement()) {
        auto ret = searchDFRecursive(element->FirstChildElement(), find_el_name, num_occurrence); 
        if (ret) {
            return ret;
        }
    }

    if (element->NextSiblingElement()) {
        auto ret = searchDFRecursive(element->NextSiblingElement(), find_el_name, num_occurrence); 
        if (ret) {
            return ret;
        }
    }

    return nullptr;
}

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

struct CompilationEntry {
    std::string directory;
    std::vector<std::string> arguments;
    std::string file;
};

void to_json(nlohmann::json& j, const CompilationEntry& c) {
    j = nlohmann::json{
        {"directory", c.directory},
        {"arguments", c.arguments},
        {"file", c.file}
    };
}

void from_json(const nlohmann::json& j, CompilationEntry& c) {
    j.at("directory").get_to(c.directory);
    j.at("arguments").get_to(c.arguments);
    j.at("file").get_to(c.file);
}

int main() {
    using namespace tinyxml2;
    using json = nlohmann::json;

    constexpr const char* include_path_to_add[] = {
        "C:\\Users\\TAmbrogini\\sls\\sls\\references\\SLS_Application\\projects\\uVision\\RTE\\Device\\ATSAME70Q21",
        "C:\\Keil_v5\\ARM\\PACK\\ARM\\CMSIS\\4.5.0\\CMSIS\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\MDK-Middleware\\7.0.0\\FileSystem\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\MDK-Middleware\\7.0.0\\Network\\Include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\SAM-ESV7_SFP\\2.2.0\\Libraries\\libchip_samv7",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\SAM-ESV7_SFP\\2.2.0\\Libraries\\libchip_samv7\\include",
        "C:\\Keil_v5\\ARM\\PACK\\Keil\\SAM-E_DFP\\2.1.3\\include"
    };

    constexpr const char* defines_to_add[] = {
        "_RTE_",
        "__UVISION_VERSION=\"517\"",
        "__CC_ARM"
    };

    constexpr const char* additional_defines[] = {
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

    std::string_view keil_project_filename { "SLS_Application.uvprojx" };

    fs::path keil_project_file { keil_project_filename };
    fs::path keil_project_file_abs { fs::absolute(keil_project_file).string() };

    XMLDocument doc;
    //TODO: check for error on loadFile()
    doc.LoadFile(keil_project_file_abs.string().c_str());

    bool found {false};
    XMLElement* root = doc.RootElement();
    const char* include_path_value {};
    const char* defines_value {};

    constexpr std::string_view target_name {"SLS_release_enhanced"};
    XMLElement* target_element = findTarget(root, target_name);
    if(!target_element) {
        std::cout << "The target was not found!" << std::endl;
        return 1;
    }

    const char* include_path_string {};
    int occ {1};
    while(true) {
        XMLElement* element = searchDF(target_element, "IncludePath", occ);
        if (element) {
            if (!element->NoChildren()) {
                include_path_string = element->FirstChild()->Value();
                break;
            }
        } else {
            std::cout << "The element: IncludePath does not exist" << std::endl;
            return 2;
        }
        ++occ;
    }

    const char* defines_string {};
    occ = {1};
    while(true) {
        XMLElement* element = searchDF(target_element, "Define", occ);
        if (element) {
            if (!element->NoChildren()) {
                defines_string = element->FirstChild()->Value();
                break;
            }
        } else {
            std::cout << "The element: Define does not exist" << std::endl;
            return 3;
        }
        ++occ;
    }

    std::string include_path {include_path_string};

    const std::regex re(R"([;]+)");
    std::sregex_token_iterator it{ include_path.begin(),
                             include_path.end(), re, -1 };

    std::vector<std::string> tokenized{ it, {} };

    tokenized.erase(
        std::remove_if(tokenized.begin(),
                            tokenized.end(),
                       [](std::string const& s) {
                           return s.size() == 0;
                       }),
        tokenized.end());

    for (int i = 0; i < sizeof(include_path_to_add)/sizeof(char*); ++i) {
        tokenized.push_back(include_path_to_add[i]);
    }

    //TODO: is there a way to not create another string everytime?
    for (int i = 0; i < tokenized.size(); ++i) {
        tokenized[i] = "-I" + tokenized[i];
    }

    const std::regex re_define(R"([,]+)");
    std::string defines {defines_string};
    std::sregex_token_iterator it_define {defines.begin(), defines.end(),
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

    std::ranges::for_each(tokenized_defines, [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
            return !std::isspace(ch);
        }));
    });

    //TODO: is there a way to not create another string everytime?
    for (int i = 0; i < tokenized_defines.size(); ++i) {
        tokenized_defines[i] = "-D" + tokenized_defines[i];
    }

    for (int i = 0; i < sizeof(additional_defines)/sizeof(char*); ++i) {
        tokenized_defines.push_back(additional_defines[i]);
    }

    tokenized.insert( tokenized.end(), tokenized_defines.begin(), tokenized_defines.end() );
    //TODO: now we have to take all the files.
    //I can just take all the filename element under the target tag.
    constexpr std::string_view filepath_tag {"FilePath"};
    int count {1};
    const XMLElement* filepath_element {};
    std::vector<const char*> filepaths {};
    std::vector<CompilationEntry> entries {};

    do {
        filepath_element = searchDF(target_element, filepath_tag, count);
        ++count;
        if (filepath_element) {
            const char* filepath = filepath_element->FirstChild()->Value();
            filepaths.push_back(filepath);
        }
    } while (filepath_element);

    //TODO: i could also pass the directory value on the argument line.
    fs::path current_dir { "." };
    //TODO: use a real value.
    //fs::path current_dir_abs {fs::absolute(current_dir).string()};
    const std::string current_dir_abs {"C:\\Users\\TAmbrogini\\sls\\sls\\references\\SLS_Application\\projects\\uVision"};
    for (auto filepath : filepaths) {
        entries.push_back(CompilationEntry{
                .directory = current_dir_abs,
                .arguments = tokenized,
                .file = filepath
                });
    }

    json j(entries);
    std::ofstream o("compile_commands.json");
    o << std::setw(4) << j << std::endl;
}

