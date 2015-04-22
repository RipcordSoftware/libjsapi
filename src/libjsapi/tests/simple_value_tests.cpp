#include <gtest/gtest.h>

#include "../libjsapi.h"

#include <string>
#include <vector>
#include <sstream>

rs::jsapi::Runtime rt_;

class SimpleValueTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }    
};

TEST_F(SimpleValueTests, test1) {
    rs::jsapi::Value value(rt_);
    
    ASSERT_TRUE(value.isUndefined());
    ASSERT_STREQ("undefined", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test2) {
    rs::jsapi::Value value(rt_, 42);
    
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    ASSERT_STREQ("42", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test3) {
    rs::jsapi::Value value(rt_, 3.14159);
    
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    ASSERT_STREQ("3.14159", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test4) {
    rs::jsapi::Value value(rt_, true);
    
    ASSERT_TRUE(value.isBoolean());
    ASSERT_TRUE(value.toBoolean());
    ASSERT_STREQ("true", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test5) {
    rs::jsapi::Value value(rt_, false);
    
    ASSERT_TRUE(value.isBoolean());
    ASSERT_FALSE(value.toBoolean());
    ASSERT_STREQ("false", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test6) {
    rs::jsapi::Value value(rt_, "hello world");
    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test7) {
    std::string str("hello world");
    rs::jsapi::Value value(rt_, str);
    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test7b) {
    std::string str(512, '1');
    rs::jsapi::Value value(rt_, str);
    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ(str.c_str(), value.ToString().c_str());
}

TEST_F(SimpleValueTests, test8) {
    rs::jsapi::Value value(rt_);            
    
    value.set(42);
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    ASSERT_STREQ("42", value.ToString().c_str());
    
    value.set(3.14159);
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    ASSERT_STREQ("3.14159", value.ToString().c_str());
    
    value.set(true);
    ASSERT_TRUE(value.isBoolean());
    ASSERT_TRUE(value.toBoolean());
    ASSERT_STREQ("true", value.ToString().c_str());
    
    value.set(false);
    ASSERT_TRUE(value.isBoolean());
    ASSERT_FALSE(value.toBoolean());
    ASSERT_STREQ("false", value.ToString().c_str());
    
    value.setUndefined();
    ASSERT_TRUE(value.isUndefined());
    ASSERT_STREQ("undefined", value.ToString().c_str());
    
    value.set("hello world");
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
    
    value.setNull();
    ASSERT_TRUE(value.isNull());
    ASSERT_STREQ("null", value.ToString().c_str());        
    
    std::string str("hello world");
    value.set(str);    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test9) {
    rs::jsapi::Value value(rt_, 42);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("42", ss.str().c_str());
}

TEST_F(SimpleValueTests, test10) {
    rs::jsapi::Value value(rt_, true);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("true", ss.str().c_str());
}

TEST_F(SimpleValueTests, test11) {
    rs::jsapi::Value value(rt_, false);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("false", ss.str().c_str());
}

TEST_F(SimpleValueTests, test12) {
    rs::jsapi::Value value(rt_, 3.14159);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("3.14159", ss.str().c_str());
}

TEST_F(SimpleValueTests, test13) {
    rs::jsapi::Value value(rt_, "hello world");            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("hello world", ss.str().c_str());
}

TEST_F(SimpleValueTests, test14) {
    std::string str(512, '1');
    rs::jsapi::Value value(rt_, str);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ(str.c_str(), ss.str().c_str());
}

TEST_F(SimpleValueTests, test15) {
    rs::jsapi::Value value(rt_, "hello world");            
    
    JS::RootedString rv(rt_, value.toString());
    auto len = JS_GetStringLength(rv);
    ASSERT_EQ(11, len);
    
    std::vector<char> buffer(len);
    JS_EncodeStringToBuffer(rt_, rv, &buffer[0], len);
    std::string str(&buffer[0], len);
    ASSERT_STREQ("hello world", str.c_str());
}

TEST_F(SimpleValueTests, test16) {
    JS::RootedObject obj(rt_);
    
    rs::jsapi::Value value(rt_, obj);
    ASSERT_TRUE(value.isObject());
}

TEST_F(SimpleValueTests, test17) {
    JS::RootedObject obj(rt_, JS_NewObject(rt_, nullptr, JS::NullPtr(), JS::NullPtr()));
    
    rs::jsapi::Value value(rt_, obj);
    ASSERT_TRUE(value.isObject());
}

TEST_F(SimpleValueTests, test18) {
    JS::RootedObject obj(rt_, JS_NewArrayObject(rt_, 0));
    
    rs::jsapi::Value value(rt_, obj);
    ASSERT_TRUE(value.isArray());
}

TEST_F(SimpleValueTests, test19) {
    rs::jsapi::Value value(rt_);
    
    JS::RootedObject obj(rt_);
    value.set(obj);
    ASSERT_TRUE(value.isObject());
}

TEST_F(SimpleValueTests, test20) {
    rs::jsapi::Value value(rt_, 42);
    
    JS::RootedObject obj(rt_);
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isNumber());
}

TEST_F(SimpleValueTests, test21) {
    rs::jsapi::Value value(rt_, "hello world");
    
    JS::RootedObject obj(rt_);
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isString());
}

TEST_F(SimpleValueTests, test22) {
    rs::jsapi::Value value(rt_, true);
    
    JS::RootedObject obj(rt_);
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isBoolean());
}

TEST_F(SimpleValueTests, test23) {
    rs::jsapi::Value value(rt_, 3.14159);
    
    JS::RootedObject obj(rt_);
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isNumber());
}