#include <gtest/gtest.h>

#include "../libjsapi.h"

#include <cstring>
#include <string>
#include <sstream>

rs::jsapi::Runtime rt_;

class SimpleDynamicArrayTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(SimpleDynamicArrayTests, test1) {
    JS::RootedObject array(rt_);    
    rs::jsapi::DynamicArray::Create(rt_,
        [](int index, rs::jsapi::Value& value) { value.set(42); return true; },
        nullptr,
        []() { return 1; },
        array);
    ASSERT_TRUE(array);
    
    rs::jsapi::Value value(rt_);
    ASSERT_TRUE(JS_GetElement(rt_, array, 0, value));
    ASSERT_EQ(42, value.toInt32());
    
    uint32_t length = -1;
    ASSERT_TRUE(JS_GetArrayLength(rt_, array, &length));
    ASSERT_EQ(1, length);    
}