#include "cli.hpp"
#include "jq_utils.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    try {
        JqUtils::initialize();
        auto args = parseQueryArgs(argc, argv, "Transform each JSON line using jq query");
        std::string line;
        while (std::getline(std::cin, line)) {
            try {
                auto result = JqUtils::evaluateJq(line, args.query);
                std::cout << result.dump() << std::endl;
            } catch (...) {
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