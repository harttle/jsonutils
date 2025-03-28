#pragma once

// Include all components
#include "cli.hpp"
#include "json.hpp"
#include "io.hpp"
#include "jq_utils.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <nlohmann/json.hpp>
#include <jq.h>

namespace jsonutils {

class JsonUtils {
public:
    // Constants
    static constexpr bool TRUE = true;

    // Common utilities for JSON processing
    static nlohmann::json parseLine(const std::string& line);
    static std::string stringify(const nlohmann::json& j);
    static bool isValidJson(const std::string& line);
    
    // I/O utilities with backpressure handling
    static void setupIO(std::istream& in, std::ostream& out, size_t buffer_size = 8192);
    static std::string readLine(std::istream& in);
    static void writeLine(std::ostream& out, const std::string& line);
    static void flushOutput(std::ostream& out);

    // jq utilities
    static bool evaluateJq(const std::string& json, const std::string& query, std::string& result);
    static bool evaluateJq(const std::string& json, const std::string& query, nlohmann::json& result);
    
    // Value checking
    static bool isTruthy(const nlohmann::json& j);
};

} // namespace jsonutils 