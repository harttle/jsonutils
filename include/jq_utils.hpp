#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <jq.h>

class JqUtils {
public:
    // Initialize and cleanup
    static void initialize();
    static void cleanup();
    
    // jq evaluation
    static nlohmann::json evaluateJq(const std::string& input, const std::string& filter);
    
    // Value checking
    static bool isTruthy(const nlohmann::json& value);

private:
    static jq_state* jq;
}; 