#include "io_utils.hpp"
#include <vector>

namespace jsonutils {

void IoUtils::setupIO(std::istream& in, std::ostream& out, size_t buffer_size) {
    // Set input buffer size
    static std::vector<char> in_buffer(buffer_size);
    in.rdbuf()->pubsetbuf(in_buffer.data(), buffer_size);
    
    // Set output buffer size
    static std::vector<char> out_buffer(buffer_size);
    out.rdbuf()->pubsetbuf(out_buffer.data(), buffer_size);
    
    // Disable input/output synchronization for better performance
    std::ios_base::sync_with_stdio(false);
}

std::string IoUtils::readLine(std::istream& in) {
    std::string line;
    std::getline(in, line);
    return line;
}

void IoUtils::writeLine(std::ostream& out, const std::string& line) {
    out << line << std::endl;
}

void IoUtils::flushOutput(std::ostream& out) {
    out << std::flush;
}

} // namespace jsonutils 