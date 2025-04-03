#include "cli.hpp"
#include "jq_utils.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        JqUtils::initialize();
        auto args = parseQueryArgs(argc, argv, "Filter JSON lines using jq query");
        std::string line;
        while (std::getline(std::cin, line)) {
            try {
                auto result = JqUtils::evaluateJq(line, args.query);
                if (JqUtils::isTruthy(result)) {
                    std::cout << line << std::endl;
                }
            } catch (...) {
                // Skip invalid JSON lines
                continue;
            }
        }
        JqUtils::cleanup();
    } catch (const std::exception& e) {
        JqUtils::cleanup();
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 