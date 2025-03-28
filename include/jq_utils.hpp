#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <jq.h>

namespace jsonutils {

class JqUtils {
public:
    // jq evaluation
    static bool evaluateJq(const std::string& json, const std::string& query, std::string& result);
    static bool evaluateJq(const std::string& json, const std::string& query, nlohmann::json& result);
    
    // Value checking
    static bool isTruthy(const nlohmann::json& j);

    static nlohmann::json parseLine(const std::string& line);
    static nlohmann::json evaluateJq(const std::string& input, const std::string& filter);
};

} // namespace jsonutils 