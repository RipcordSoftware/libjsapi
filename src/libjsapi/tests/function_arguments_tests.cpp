#include <gtest/gtest.h>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class FunctionArgumentsTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }   
};

TEST_F(FunctionArgumentsTests, test1) {
    rs::jsapi::FunctionArguments args(rt_);
    
    ASSERT_EQ(0, args.getLength());
    args.Clear();
    ASSERT_EQ(0, args.getLength());
    ASSERT_TRUE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test2) {
    rs::jsapi::FunctionArguments args(rt_);   
    
    args.Append(99);
 
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());
    args.Clear();
    ASSERT_TRUE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test3) {
    rs::jsapi::FunctionArguments args(rt_);   
    
    args.Append(99);
 
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());
    ASSERT_TRUE(args[0].isInt32());
    ASSERT_EQ(99, args[0].toInt32());
}

TEST_F(FunctionArgumentsTests, test4) {
    rs::jsapi::FunctionArguments args(rt_);   
    
    args.Append(99);
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());

    args.Append("Hello world");
    ASSERT_EQ(2, args.getLength());
    ASSERT_FALSE(args.Empty());
    
    ASSERT_TRUE(args[0].isInt32());
    ASSERT_EQ(99, args[0].toInt32());    
    
    ASSERT_TRUE(args[1].isString());
    rs::jsapi::Value result(rt_, args[1].toString());    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("Hello world", result.ToString().c_str());
    
    args.Clear();
    ASSERT_TRUE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test5) {
    rs::jsapi::FunctionArguments args(rt_);   
    
    JS::RootedObject obj(rt_);
    args.Append(obj);
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());
    
    JS::RootedValue value(rt_);
    args.Append(value);
    ASSERT_EQ(2, args.getLength());
    ASSERT_FALSE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test6) {        
    rs::jsapi::FunctionArguments args(rt_);
    
    ASSERT_THROW({        
        args[0].isString();
    }, rs::jsapi::FunctionArgumentsIndexException);
    
    bool thrown = false;
    try {
        args[0].isString();
    } catch (const rs::jsapi::FunctionArgumentsIndexException& ex) {
        ASSERT_NE(nullptr, ex.what());
        thrown = true;
    }
    
    args.Append(true);
    ASSERT_EQ(1, args.getLength());
    ASSERT_TRUE(args[0].isBoolean());
    ASSERT_THROW({ args[1].isBoolean(); }, rs::jsapi::FunctionArgumentsIndexException);
}