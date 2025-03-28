#include "jsonutils.hpp"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    std::vector<std::string> args;
    // Skip program name
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    
    return jsonutils::JsonUtils::process(args);
} 