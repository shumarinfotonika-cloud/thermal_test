#include "ConfigReader.h"
#include <fstream>
#include <sstream>
#include <iostream>


ConfigReader::ConfigReader(const std::string& config_file) {
    std::ifstream file(config_file);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file: " + config_file);
    }

    std::string line;
    std::string current_section;
    int subsection_counter = 0;
    std::string current_subsection;

    while (std::getline(file, line)) {
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        if (line.empty()) continue;

        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        line.erase(0, line.find_first_not_of(" \t\r\n"));

        if (line.front() == '[' && line.back() == ']') {
            if (line[1] == '/') {
                if (!current_subsection.empty()) {
                    current_subsection.clear();
                    subsection_counter++;
                } else {
                    current_section.clear();
                    subsection_counter = 0;
                }
            } else {
                size_t pos = line.find(']');
                std::string section_name = line.substr(1, pos - 1);

                if (!current_section.empty()) {
                    current_subsection = current_section + "." + section_name + "." + std::to_string(subsection_counter);
                } else {
                    current_section = section_name;
                    subsection_counter = 0;
                }
            }
        } else {
            size_t equal_pos = line.find('=');
            if (equal_pos == std::string::npos) continue;

            std::string key = line.substr(0, equal_pos);
            std::string value = line.substr(equal_pos + 1);

            key.erase(key.find_last_not_of(" \t") + 1);
            key.erase(0, key.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            if (!current_subsection.empty()) {
                sections_[current_subsection][key] = value;
            } else {
                sections_[current_section][key] = value;
            }
        }
    }
}

bool ConfigReader::hasSection(const std::string& section) const {
    return sections_.find(section) != sections_.end();
}

std::string ConfigReader::getString(const std::string& section, const std::string& key) const {
    auto section_it = sections_.find(section);
    if (section_it == sections_.end()) {
        throw std::runtime_error("Section not found in config: " + section);
    }

    auto param_it = section_it->second.find(key);
    if (param_it == section_it->second.end()) {
        throw std::runtime_error("Key not found in config: " + key);
    }

    return param_it->second;
}

double ConfigReader::getDouble(const std::string& section, const std::string& key) const {
    return std::stod(getString(section, key));
}

int ConfigReader::getInt(const std::string& section, const std::string& key) const {
    return std::stoi(getString(section, key));
}

std::vector<std::map<std::string, std::string>> ConfigReader::getSubsections(const std::string& section) const {
    std::vector<std::map<std::string, std::string>> subsections;
    for (const auto& [key, value_map] : sections_) {
        if (key.find(section + ".") == 0 || key == section) {
            subsections.push_back(value_map);
        }
    }
    return subsections;
}

std::vector<std::string> ConfigReader::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string item;
    while (std::getline(iss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

std::string ConfigReader::replacePlaceholder(const std::string& str, const std::string& placeholder, const std::string& value) {
    std::string result = str;
    size_t pos = result.find(placeholder);
    if (pos != std::string::npos) {
        result.replace(pos, placeholder.length(), value);
    }
    return result;
}

std::pair<int, int> ConfigReader::getPairInt(const std::string& section, const std::string& key, char delimiter) const {
    std::string str = getString(section, key);
    auto parts = splitString(str, delimiter);
    if (parts.size() != 2) {
        throw std::runtime_error("Invalid pair format for key: " + key);
    }
    return {std::stoi(parts[0]), std::stoi(parts[1])};
}

std::pair<double, double> ConfigReader::getPairDouble(const std::string& section, const std::string& key, char delimiter) const {
    std::string str = getString(section, key);
    auto parts = splitString(str, delimiter);
    if (parts.size() != 2) {
        throw std::runtime_error("Invalid pair format for key: " + key);
    }
    return {std::stod(parts[0]), std::stod(parts[1])};
}
