#include <gtest/gtest.h>
#include "jsonutils.hpp"
#include <sstream>
#include <thread>
#include <chrono>

TEST(JwhereTest, BasicFilter) {
    std::string input = R"({"name": "John", "age": 35})";
    std::string query = ".age > 30";
    std::string result;
    
    EXPECT_TRUE(jsonutils::JsonUtils::evaluateJq(input, query, result));
    EXPECT_EQ(result, "true");
}

TEST(JwhereTest, StringComparison) {
    std::string input = R"({"name": "John", "type": "user"})";
    std::string query = ".type == \"user\"";
    std::string result;
    
    EXPECT_TRUE(jsonutils::JsonUtils::evaluateJq(input, query, result));
    EXPECT_EQ(result, "true");
}

TEST(JwhereTest, ComplexCondition) {
    std::string input = R"({"user": {"age": 35, "type": "premium"}})";
    std::string query = ".user.age > 30 and .user.type == \"premium\"";
    std::string result;
    
    EXPECT_TRUE(jsonutils::JsonUtils::evaluateJq(input, query, result));
    EXPECT_EQ(result, "true");
}

TEST(JwhereTest, InvalidJson) {
    std::string input = "invalid json";
    std::string query = ".age > 30";
    std::string result;
    
    EXPECT_FALSE(jsonutils::JsonUtils::evaluateJq(input, query, result));
}

TEST(JwhereTest, InvalidQuery) {
    std::string input = R"({"age": 30})";
    std::string query = "invalid query";
    std::string result;
    
    EXPECT_FALSE(jsonutils::JsonUtils::evaluateJq(input, query, result));
}

// Test backpressure handling with small buffer
TEST(JwhereTest, BackpressureHandling) {
    std::stringstream input, output;
    
    // Setup with very small buffer to simulate backpressure
    jsonutils::JsonUtils::setupIO(input, output, 16);
    
    // Generate large input
    std::string large_json = R"({"data": ")" + std::string(1000, 'x') + R"("})";
    input << large_json << std::endl;
    
    // Write to output stream
    jsonutils::JsonUtils::writeLine(output, large_json);
    
    // Verify output was written correctly despite backpressure
    std::string result;
    std::getline(output, result);
    EXPECT_EQ(result, large_json);
}

// Test backpressure with multiple lines
TEST(JwhereTest, MultipleLinesBackpressure) {
    std::stringstream input, output;
    
    // Setup with very small buffer
    jsonutils::JsonUtils::setupIO(input, output, 16);
    
    // Generate multiple lines
    for (int i = 0; i < 10; ++i) {
        std::string json = R"({"id": )" + std::to_string(i) + R"(, "data": ")" + std::string(100, 'x') + R"("})";
        input << json << std::endl;
        jsonutils::JsonUtils::writeLine(output, json);
    }
    
    // Verify all lines were written correctly
    for (int i = 0; i < 10; ++i) {
        std::string result;
        std::getline(output, result);
        EXPECT_TRUE(result.find("\"id\": " + std::to_string(i)) != std::string::npos);
    }
} 