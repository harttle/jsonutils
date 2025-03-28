#pragma once

#include <string>
#include <iostream>

namespace jsonutils {

class IoUtils {
public:
    // I/O with backpressure handling
    static void setupIO(std::istream& in, std::ostream& out, size_t buffer_size = 8192);
    static std::string readLine(std::istream& in);
    static void writeLine(std::ostream& out, const std::string& line);
    static void flushOutput(std::ostream& out);
};

} // namespace jsonutils 