#include "xml_analysis.h"

#include <fmt/base.h>

using namespace tinyxml2;

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

std::string_view getElementValue(XMLElement* target_element, std::string_view search_name) {
    int occ = {1};
    while(true) {
        XMLElement* element = searchDF(target_element, search_name.data(), occ);
        if (element) {
            if (!element->NoChildren()) {
                std::string_view result_value { element->FirstChild()->Value() };
                return result_value;
            }
        } else {
            fmt::print("The element {} does not exist\n", search_name);
            return {};
        }
        ++occ;
    }
}

std::vector<std::string_view> getAllValues(tinyxml2::XMLElement* root_element, std::string_view search_tag) {
    const XMLElement* filepath_element {};
    std::vector<std::string_view> ret {};

    int count {1};
    do {
        filepath_element = searchDF(root_element, search_tag, count);
        ++count;
        if (filepath_element) {
            const char* filepath = filepath_element->FirstChild()->Value();
            ret.push_back(filepath);
        }
    } while (filepath_element);

    return ret;
}

