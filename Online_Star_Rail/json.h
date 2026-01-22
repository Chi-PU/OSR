#include <string>
#include <sstream>
#include <map>
#include <vector>

class SimpleJSON {
private:
    std::map<std::string, std::string> data;

    // Escape special characters in strings
    std::string escape_string(const std::string& str);

public:
    // Add string value
    void add(const std::string& key, const std::string& value);

	//Add string value for const char*
    void add(const std::string& key, const char* value);

    // Add integer value
    void add(const std::string& key, int value);

    // Add boolean value
    void add(const std::string& key, bool value);
    // Add null value
    void add_null(const std::string& key);

    // Build JSON string
    std::string build();

    // Clear all data
    void clear();
};

