#include "jq_utils.hpp"
#include "json_parser.hpp"
#include <jq.h>

namespace jsonutils {

nlohmann::json JqUtils::parseLine(const std::string& line) {
    return nlohmann::json::parse(line);
}

bool JqUtils::isTruthy(const nlohmann::json& value) {
    if (value.is_boolean()) return value.get<bool>();
    if (value.is_number()) return value.get<double>() != 0;
    if (value.is_string()) return !value.get<std::string>().empty();
    if (value.is_array()) return !value.empty();
    if (value.is_object()) return !value.empty();
    if (value.is_null()) return false;
    return true;
}

nlohmann::json JqUtils::evaluateJq(const std::string& input, const std::string& filter) {
    jq_state *jq = jq_init();
    if (!jq) {
        throw std::runtime_error("Failed to initialize jq");
    }

    int compiled = jq_compile(jq, filter.c_str());
    if (!compiled) {
        jq_teardown(&jq);
        throw std::runtime_error("Failed to compile jq filter");
    }

    jv parsed = jv_parse(input.c_str());
    if (jv_is_invalid(parsed)) {
        jq_teardown(&jq);
        throw std::runtime_error("Failed to parse input JSON");
    }

    jv result = jq_next(jq, parsed);
    std::string output = jv_dump_string(result);
    jv_free(result);
    jq_teardown(&jq);

    return parseLine(output);
}

} // namespace jsonutils 