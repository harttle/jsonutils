#include "jq_utils.hpp"
#include <nlohmann/json.hpp>
#include <CLI/CLI.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct JoinArgs {
    std::string stdin_key;
    std::string rhs_key;
    std::string rhs_path;
};

JoinArgs parseArgs(int argc, char* argv[]) {
    CLI::App app{"Join JSON lines from stdin with rhs based on jq expressions"};
    
    JoinArgs args;
    args.stdin_key = ".id";  // default key
    args.rhs_key = ".id";  // default key
    
    app.add_option("-s,--stdin-key", args.stdin_key, 
                  "Expression to extract key from stdin (default: .id)");
    app.add_option("-f,--rhs-key", args.rhs_key, 
                  "Expression to extract key from rhs (default: .id)");
    app.add_option("--rhs", args.rhs_path, 
                  "Second file to join with (default: stdin)");
    
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::cerr << app.help() << std::endl;
        throw std::runtime_error(e.what());
    }
    
    return args;
}

int main(int argc, char* argv[]) {
    try {
        JqUtils::initialize();
        JoinArgs args = parseArgs(argc, argv);
        std::map<std::string, std::string> lhs;

        // Read first file (stdin)
        std::string line;
        while (std::getline(std::cin, line)) {
            try {
                auto result = JqUtils::evaluateJq(line, args.stdin_key);
                std::string key = result.dump();
                lhs[key] = line;
            } catch (...) {
                continue;
            }
        }

        // Read second file if provided
        std::istream* rhs = &std::cin;
        std::ifstream rhs_stream;
        if (!args.rhs_path.empty()) {
            rhs_stream.open(args.rhs_path);
            if (!rhs_stream) {
                JqUtils::cleanup();
                std::cerr << "Error: Cannot open " << args.rhs_path << std::endl;
                return 1;
            }
            rhs = &rhs_stream;
        }

        while (std::getline(*rhs, line)) {
            try {
                auto result = JqUtils::evaluateJq(line, args.rhs_key);
                std::string key = result.dump();
                auto it = lhs.find(key);
                if (it != lhs.end()) {
                    std::cout << it->second << std::endl;
                }
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