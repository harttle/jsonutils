#include <gtest/gtest.h>
#include "jsonutils.hpp"
#include <sstream>

TEST(JjoinTest, BasicJoin) {
    std::string input1 = R"({"id": 1, "name": "John"})";
    std::string input2 = R"({"id": 1, "age": 30})";
    
    auto json1 = jsonutils::JsonUtils::parseLine(input1);
    auto json2 = jsonutils::JsonUtils::parseLine(input2);
    
    EXPECT_TRUE(json1.contains("id"));
    EXPECT_TRUE(json2.contains("id"));
    EXPECT_EQ(json1["id"], json2["id"]);
}

TEST(JjoinTest, MissingKey) {
    std::string input1 = R"({"name": "John"})";
    std::string input2 = R"({"id": 1, "age": 30})";
    
    auto json1 = jsonutils::JsonUtils::parseLine(input1);
    auto json2 = jsonutils::JsonUtils::parseLine(input2);
    
    EXPECT_FALSE(json1.contains("id"));
    EXPECT_TRUE(json2.contains("id"));
}

TEST(JjoinTest, DifferentValues) {
    std::string input1 = R"({"id": 1, "name": "John"})";
    std::string input2 = R"({"id": 2, "name": "Jane"})";
    
    auto json1 = jsonutils::JsonUtils::parseLine(input1);
    auto json2 = jsonutils::JsonUtils::parseLine(input2);
    
    EXPECT_NE(json1["id"], json2["id"]);
}

TEST(JjoinTest, InvalidJson) {
    std::string input = "invalid json";
    
    EXPECT_THROW(jsonutils::JsonUtils::parseLine(input), nlohmann::json::parse_error);
}

TEST(JjoinTest, ComplexJoin) {
    std::string input1 = R"({"user_id": 1, "name": "John", "role": "admin"})";
    std::string input2 = R"({"user_id": 1, "age": 30, "department": "IT"})";
    
    auto json1 = jsonutils::JsonUtils::parseLine(input1);
    auto json2 = jsonutils::JsonUtils::parseLine(input2);
    
    EXPECT_EQ(json1["user_id"], json2["user_id"]);
    EXPECT_TRUE(json1.contains("role"));
    EXPECT_TRUE(json2.contains("department"));
} 