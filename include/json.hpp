#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace jsonutils {

// JSON utilities
class JsonUtils {
public:
    // JSON parsing and stringification
    static nlohmann::json parseLine(const std::string& line);
    static std::string stringify(const nlohmann::json& j);
    static bool isValidJson(const std::string& line);
    
    // Value checking
    static bool isTruthy(const nlohmann::json& j);
    
    // jq evaluation
    static bool evaluateJq(const std::string& json, const std::string& query, std::string& result);
    static bool evaluateJq(const std::string& json, const std::string& query, nlohmann::json& result);
};

} // namespace jsonutils 