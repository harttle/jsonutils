#include "jsonutils.hpp"
#include <CLI/CLI.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct JoinArgs {
    std::string stdin_key;
    std::string file2_key;
    std::string file2_path;
};

JoinArgs parseArgs(int argc, char* argv[]) {
    CLI::App app{"Join JSON lines from stdin with file2 based on jq expressions"};
    
    JoinArgs args;
    args.stdin_key = ".id";  // default key
    args.file2_key = ".id";  // default key
    
    app.add_option("-s,--stdin-key", args.stdin_key, 
                  "Expression to extract key from stdin (default: .id)");
    app.add_option("-f,--file2-key", args.file2_key, 
                  "Expression to extract key from file2 (default: .id)");
    app.add_option("--file2", args.file2_path, 
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
        JoinArgs args = parseArgs(argc, argv);
        std::map<std::string, std::string> first_file;

        // Read first file (stdin)
        std::string line;
        while (std::getline(std::cin, line)) {
            try {
                nlohmann::json result;
                if (jsonutils::JsonUtils::evaluateJq(line, args.stdin_key, result)) {
                    std::string key = jsonutils::JsonUtils::stringify(result);
                    first_file[key] = line;
                }
            } catch (...) {
                continue;
            }
        }

        // Read second file if provided
        std::istream* file2 = &std::cin;
        std::ifstream file2_stream;
        if (!args.file2_path.empty()) {
            file2_stream.open(args.file2_path);
            if (!file2_stream) {
                std::cerr << "Error: Cannot open " << args.file2_path << std::endl;
                return 1;
            }
            file2 = &file2_stream;
        }

        while (std::getline(*file2, line)) {
            try {
                nlohmann::json result;
                if (jsonutils::JsonUtils::evaluateJq(line, args.file2_key, result)) {
                    std::string key = jsonutils::JsonUtils::stringify(result);
                    auto it = first_file.find(key);
                    if (it != first_file.end()) {
                        std::cout << it->second << std::endl;
                    }
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