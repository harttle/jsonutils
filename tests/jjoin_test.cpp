#include <gtest/gtest.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <fstream>

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

// Helper function to create a temporary file with content
std::string createTempFile(const std::string& content) {
    char filename[] = "/tmp/jjoin_test_XXXXXX";
    int fd = mkstemp(filename);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temp file");
    }
    
    FILE* file = fdopen(fd, "w");
    if (!file) {
        close(fd);
        throw std::runtime_error("Failed to open temp file");
    }
    
    fprintf(file, "%s", content.c_str());
    fclose(file);
    
    return std::string(filename);
}

TEST(JjoinTest, BasicJoin) {
    std::string input1 = R"({"id": 1, "name": "John"}
{"id": 2, "name": "Jane"})";
    std::string input2 = R"({"id": 1, "age": 30}
{"id": 2, "age": 25})";
    
    std::string file2 = createTempFile(input2);
    std::string expected = R"({"id": 1, "name": "John"}
{"id": 2, "name": "Jane"})";
    
    std::string output = exec("./jjoin --rhs " + file2, input1);
    EXPECT_EQ(output, expected + "\n");
    remove(file2.c_str());
}

TEST(JjoinTest, CustomKeys) {
    std::string input1 = R"({"user_id": 1, "name": "John"}
{"user_id": 2, "name": "Jane"})";
    std::string input2 = R"({"id": 1, "age": 30}
{"id": 2, "age": 25})";
    
    std::string file2 = createTempFile(input2);
    std::string expected = R"({"user_id": 1, "name": "John"}
{"user_id": 2, "name": "Jane"})";
    
    std::string output = exec("./jjoin -s .user_id -f .id --rhs " + file2, input1);
    EXPECT_EQ(output, expected + "\n");
    remove(file2.c_str());
}

TEST(JjoinTest, NoMatches) {
    std::string input1 = R"({"id": 1, "name": "John"}
{"id": 2, "name": "Jane"})";
    std::string input2 = R"({"id": 3, "age": 30}
{"id": 4, "age": 25})";
    
    std::string file2 = createTempFile(input2);
    std::string output = exec("./jjoin --rhs " + file2, input1);
    EXPECT_EQ(output, "");
    remove(file2.c_str());
}

TEST(JjoinTest, InvalidJson) {
    std::string input1 = R"({"id": 1, "name": "John"}
invalid json
{"id": 2, "name": "Jane"})";
    std::string input2 = R"({"id": 1, "age": 30}
{"id": 2, "age": 25})";
    
    std::string file2 = createTempFile(input2);
    std::string expected = R"({"id": 1, "name": "John"}
{"id": 2, "name": "Jane"})";
    
    std::string output = exec("./jjoin --rhs " + file2, input1);
    EXPECT_EQ(output, expected + "\n");
    remove(file2.c_str());
}

TEST(JjoinTest, ComplexJoin) {
    std::string input1 = R"({"data": {"id": 1, "type": "user"}, "name": "John"}
{"data": {"id": 2, "type": "admin"}, "name": "Jane"})";
    std::string input2 = R"({"user": {"id": 1}, "age": 30}
{"user": {"id": 2}, "age": 25})";
    
    std::string file2 = createTempFile(input2);
    std::string expected = R"({"data": {"id": 1, "type": "user"}, "name": "John"}
{"data": {"id": 2, "type": "admin"}, "name": "Jane"})";
    
    std::string output = exec("./jjoin -s .data.id -f .user.id --rhs " + file2, input1);
    EXPECT_EQ(output, expected + "\n");
    remove(file2.c_str());
} 