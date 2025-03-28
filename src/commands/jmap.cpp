#include "jsonutils.hpp"
#include "command_args.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    try {
        auto args = jsonutils::parseQueryArgs(argc, argv, "Transform each JSON line using jq query");
        std::string line;
        while (std::getline(std::cin, line)) {
            try {
                nlohmann::json result;
                if (jsonutils::JsonUtils::evaluateJq(line, args.query, result)) {
                    std::cout << jsonutils::JsonUtils::stringify(result) << std::endl;
                }
            } catch (...) {
                continue;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 