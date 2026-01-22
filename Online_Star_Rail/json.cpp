#include "json.h"


    std::map<std::string, std::string> data;

    // Escape special characters in strings
    std::string SimpleJSON::escape_string(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:   result += c;
            }
        }
        return result;
    }



    // Add string value
    void SimpleJSON::add(const std::string& key, const std::string& value) {
        data[key] = "\"" + escape_string(value) + "\"";
    }

	// Add string value for const char*
    void SimpleJSON::add(const std::string& key, const char* value) {
        data[key] = "\"" + escape_string(std::string(value)) + "\"";
    }

    // Add integer value
    void SimpleJSON::add(const std::string& key, int value) {
        data[key] = std::to_string(value);
    }

    // Add boolean value
    void SimpleJSON::add(const std::string& key, bool value) {
        data[key] = value ? "true" : "false";
    }

    // Add null value
    void SimpleJSON::add_null(const std::string& key) {
        data[key] = "null";
    }

    // Build JSON string
    std::string SimpleJSON::build() {
        std::stringstream ss;
        ss << "{";

        bool first = true;
        for (const auto& pair : data) {
            if (!first) ss << ",";
            ss << "\"" << pair.first << "\":" << pair.second;
            first = false;
        }

        ss << "}";
        return ss.str();
    }

    // Clear all data
    void SimpleJSON::clear() {
        data.clear();
    }


