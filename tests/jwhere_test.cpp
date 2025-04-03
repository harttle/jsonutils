#include <gtest/gtest.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>

// Helper function to execute command and get output
std::string exec(const std::string& cmd, const std::string& input = "") {
    std::array<char, 128> buffer;
    std::string result;

    // Create pipes for input and output
    FILE* pipe = popen((cmd + " 2>&1").c_str(), "w");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Write input to pipe if provided
    if (!input.empty()) {
        fprintf(pipe, "%s", input.c_str());
    }

    // Close pipe and get output
    pclose(pipe);

    pipe = popen((cmd + " 2>&1").c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}

TEST(JwhereTest, BasicFilter) {
    std::string input = R"({"name": "John", "age": 30}
{"name": "Jane", "age": 25})";
    std::string expected = R"({"name": "John", "age": 30})";
    
    std::string output = exec("./jwhere '.age > 25'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JwhereTest, StringComparison) {
    std::string input = R"({"name": "John", "type": "admin"}
{"name": "Jane", "type": "user"})";
    std::string expected = R"({"name": "Jane", "type": "user"})";
    
    std::string output = exec("./jwhere '.type == \"user\"'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JwhereTest, ComplexCondition) {
    std::string input = R"({"name": "John", "age": 30, "type": "premium"}
{"name": "Jane", "age": 25, "type": "basic"}
{"name": "Bob", "age": 35, "type": "premium"})";
    std::string expected = R"({"name": "John", "age": 30, "type": "premium"}
{"name": "Bob", "age": 35, "type": "premium"})";
    
    std::string output = exec("./jwhere '.age > 25 and .type == \"premium\"'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JwhereTest, InvalidJson) {
    std::string input = R"({"name": "John", "age": 30}
invalid json
{"name": "Jane", "age": 25})";
    std::string expected = R"({"name": "John", "age": 30}
{"name": "Jane", "age": 25})";
    
    std::string output = exec("./jwhere 'true'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JwhereTest, InvalidQuery) {
    std::string input = R"({"name": "John", "age": 30})";
    std::string output = exec("./jwhere 'invalid query'", input);
    EXPECT_TRUE(output.find("Error") != std::string::npos);
}

TEST(JwhereTest, NoMatches) {
    std::string input = R"({"name": "John", "age": 30}
{"name": "Jane", "age": 25})";
    
    std::string output = exec("./jwhere '.age > 40'", input);
    EXPECT_EQ(output, "");
}

TEST(JwhereTest, AllMatches) {
    std::string input = R"({"name": "John", "age": 30}
{"name": "Jane", "age": 25})";
    std::string expected = R"({"name": "John", "age": 30}
{"name": "Jane", "age": 25})";
    
    std::string output = exec("./jwhere '.age > 0'", input);
    EXPECT_EQ(output, expected + "\n");
} 