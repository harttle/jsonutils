#include "jsonutils.hpp"
#include <sstream>
#include <algorithm>

namespace jsonutils {

nlohmann::json JsonUtils::parseLine(const std::string& line) {
    return nlohmann::json::parse(line);
}

std::string JsonUtils::stringify(const nlohmann::json& j) {
    return j.dump();
}

bool JsonUtils::isValidJson(const std::string& line) {
    try {
        nlohmann::json::parse(line);
        return true;
    } catch (...) {
        return false;
    }
}

void JsonUtils::setupIO(std::istream& in, std::ostream& out, size_t buffer_size) {
    // Set input buffer size
    static std::vector<char> in_buffer(buffer_size);
    in.rdbuf()->pubsetbuf(in_buffer.data(), buffer_size);
    
    // Set output buffer size
    static std::vector<char> out_buffer(buffer_size);
    out.rdbuf()->pubsetbuf(out_buffer.data(), buffer_size);
    
    // Disable input/output synchronization for better performance
    std::ios_base::sync_with_stdio(false);
}

std::string JsonUtils::readLine(std::istream& in) {
    std::string line;
    std::getline(in, line);
    return line;
}

void JsonUtils::writeLine(std::ostream& out, const std::string& line) {
    out << line << std::endl;
}

void JsonUtils::flushOutput(std::ostream& out) {
    out << std::flush;
}

bool JsonUtils::evaluateJq(const std::string& json, const std::string& query, std::string& result) {
    jq_state* jq = jq_init();
    if (!jq) return false;

    if (jq_compile(jq, query.c_str()) == 0) {
        jq_teardown(&jq);
        return false;
    }

    jv input = jv_parse(json.c_str());
    if (jv_is_invalid(input)) {
        jq_teardown(&jq);
        return false;
    }

    jv output = jq_next(jq, input);
    if (jv_is_invalid(output)) {
        jq_teardown(&jq);
        return false;
    }

    result = jv_string_value(output);
    jv_free(output);
    jq_teardown(&jq);
    return true;
}

bool JsonUtils::evaluateJq(const std::string& json, const std::string& query, nlohmann::json& result) {
    std::string output;
    if (!evaluateJq(json, query, output)) {
        return false;
    }
    try {
        result = nlohmann::json::parse(output);
        return true;
    } catch (...) {
        return false;
    }
}

bool JsonUtils::isTruthy(const nlohmann::json& j) {
    if (j.is_boolean()) {
        return j.get<bool>();
    }
    
    if (j.is_null()) {
        return false;
    }
    
    if (j.is_string()) {
        return !j.get<std::string>().empty();
    }
    
    if (j.is_number()) {
        return j.get<double>() != 0;
    }
    
    if (j.is_array() || j.is_object()) {
        return !j.empty();
    }
    
    return true; // Any other type is considered truthy
}

} // namespace jsonutils 