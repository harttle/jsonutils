#pragma once

#include <string>
#include <CLI/CLI.hpp>

namespace jsonutils {

struct QueryArgs {
    std::string query;
};

// CLI argument parsing
class CliArgs {
public:
    // Query command arguments
    static QueryArgs parseQueryArgs(int argc, char* argv[], const std::string& description) {
        CLI::App app{description};
        
        QueryArgs args;
        app.add_option("query", args.query, "jq query expression")->required();
        
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError &e) {
            std::cerr << app.help() << std::endl;
            throw std::runtime_error(e.what());
        }
        
        return args;
    }
};

} // namespace jsonutils 