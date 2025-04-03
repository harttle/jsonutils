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

TEST(JmapTest, BasicMap) {
    std::string input = R"({"name": "John", "age": 30})";
    std::string expected = R"({"user":"John","years":30})";
    
    std::string output = exec("./jmap '{user: .name, years: .age}'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, MultipleLinesMap) {
    std::string input = R"({"name": "John", "age": 30}
{"name": "Jane", "age": 25})";
    std::string expected = R"({"user":"John","years":30}
{"user":"Jane","years":25})";
    
    std::string output = exec("./jmap '{user: .name, years: .age}'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, ArrayMap) {
    std::string input = R"([1, 2, 3])";
    std::string expected = R"([2,4,6])";
    
    std::string output = exec("./jmap 'map(. * 2)'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, ObjectArrayMap) {
    std::string input = R"([{"name": "John", "age": 30}, {"name": "Jane", "age": 25}])";
    std::string expected = R"([{"name":"John","age":30,"isAdult":true},{"name":"Jane","age":25,"isAdult":true}])";
    
    std::string output = exec("./jmap 'map(. + {isAdult: (.age >= 18)})'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, NestedArrayMap) {
    std::string input = R"({"items": [1, 2, 3]})";
    std::string expected = R"({"items":[2,4,6]})";
    
    std::string output = exec("./jmap '{items: (.items | map(. * 2))}'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, ComplexMap) {
    std::string input = R"({"users": [{"name": "John", "age": 30}, {"name": "Jane", "age": 25}]})";
    std::string expected = R"({"users":[{"age":30,"isAdult":true,"name":"John"},{"age":25,"isAdult":true,"name":"Jane"}]})";
    
    std::string output = exec("./jmap '{users: (.users | map({name: .name, age: .age, isAdult: (.age >= 18)}))}'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, InvalidLines) {
    std::string input = R"({"name": "John", "age": 30}
invalid json
{"name": "Jane", "age": 25})";
    std::string expected = R"({"user":"John","years":30}
{"user":"Jane","years":25})";
    
    std::string output = exec("./jmap '{user: .name, years: .age}'", input);
    EXPECT_EQ(output, expected + "\n");
}

TEST(JmapTest, InvalidMapStatement) {
    std::string input = R"({"name": "John", "age": 30})";
    std::string output = exec("./jmap 'invalid map statement'", input);
    EXPECT_TRUE(output.find("Error") != std::string::npos);
}

TEST(JmapTest, MissingMapStatement) {
    std::string input = R"({"name": "John", "age": 30})";
    std::string output = exec("./jmap", input);
    EXPECT_TRUE(output.find("Error") != std::string::npos);
} 