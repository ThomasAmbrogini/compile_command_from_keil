#pragma once

#include <string_view>
#include <vector>

#include "tinyxml2.h"


tinyxml2::XMLElement* searchDF(tinyxml2::XMLElement* element, std::string_view find_el_name, int num_occurrence);
tinyxml2::XMLElement* searchDFRecursive(tinyxml2::XMLElement* element, std::string_view find_el_name, int& num_occurrence);

std::string_view getElementValue(tinyxml2::XMLElement* target_element, std::string_view search_name);
std::vector<std::string_view> getAllValues(tinyxml2::XMLElement* root_element, std::string_view search_tag);

