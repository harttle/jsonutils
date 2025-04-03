#include "jq_utils.hpp"
#include <jq.h>
#include <jv.h>
#include <nlohmann/json.hpp>

jq_state* JqUtils::jq = nullptr;

void JqUtils::initialize() {
    if (!jq) {
        jq = jq_init();
        if (!jq) {
            throw std::runtime_error("Failed to initialize jq");
        }
    }
}

void JqUtils::cleanup() {
    if (jq) {
        jq_teardown(&jq);
    }
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
    if (!jq) {
        initialize();
    }

    // Compile the filter
    if (!jq_compile(jq, filter.c_str())) {
        throw std::runtime_error("Failed to compile jq filter");
    }

    // Parse input JSON
    jv input_jv = jv_parse_sized(input.c_str(), input.length());
    if (jv_get_kind(input_jv) == JV_KIND_INVALID) {
        jv_free(input_jv);
        throw std::runtime_error("Failed to parse input JSON");
    }

    // Process the input
    jq_start(jq, input_jv, 0);  // input_jv is consumed by jq_start

    // Get the result
    jv value = jq_next(jq);
    if (jv_get_kind(value) == JV_KIND_INVALID) {
        jv_free(value);
        throw std::runtime_error("Failed to evaluate jq filter");
    }

    // Convert to string with proper flags
    jv dumped = jv_dump_string(jv_copy(value), JV_PRINT_PRETTY);
    if (jv_get_kind(dumped) == JV_KIND_INVALID) {
        jv_free(dumped);
        jv_free(value);
        throw std::runtime_error("Failed to convert jq result to string");
    }

    // Get the string value and parse it as JSON
    const char* str = jv_string_value(dumped);
    std::string result_str = str ? str : "";
    jv_free(dumped);
    jv_free(value);

    return nlohmann::json::parse(result_str);
}