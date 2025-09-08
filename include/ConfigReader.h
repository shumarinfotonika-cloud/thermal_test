#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include <map>
#include <unordered_map>
#include <vector>

class ConfigReader {
public:
    ConfigReader(const std::string& config_file);
    std::string getString(const std::string& section, const std::string& key) const;
    double getDouble(const std::string& section, const std::string& key) const;
    int getInt(const std::string& section, const std::string& key) const;
    std::vector<std::map<std::string, std::string>> getSubsections(const std::string& section) const;
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
    static std::string replacePlaceholder(const std::string& str, const std::string& placeholder, const std::string& value);
    std::pair<int, int> getPairInt(const std::string& section, const std::string& key, char delimiter) const;
    std::pair<double, double> getPairDouble(const std::string& section, const std::string& key, char delimiter) const;

private:
    std::unordered_map<std::string, std::map<std::string, std::string>> sections_; ///< Хранилище параметров по секциям.
};

#endif // CONFIGREADER_H
