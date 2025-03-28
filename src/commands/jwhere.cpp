#include "json_parser.hpp"
#include "jq_utils.hpp"
#include "cli_args.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        auto args = jsonutils::CliArgs::parseQueryArgs(argc, argv, "Filter JSON lines using jq query");
        std::string line;
        while (std::getline(std::cin, line)) {
            try {
                nlohmann::json result;
                if (jsonutils::JqUtils::evaluateJq(line, args.query, result)) {
                    if (jsonutils::JqUtils::isTruthy(result)) {
                        std::cout << line << std::endl;
                    }
                }
            } catch (...) {
                // Skip invalid JSON lines
                continue;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 