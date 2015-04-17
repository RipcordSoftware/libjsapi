#include <gtest/gtest.h>

#include "../libjsapi.h"

#include <cstring>
#include <string>

rs::jsapi::Runtime rt_;

class SimpleObjectTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(SimpleObjectTests, test1) {
    JS::RootedObject obj(rt_);
    rs::jsapi::Object::Create(rt_, {}, nullptr, nullptr, {}, obj);
    ASSERT_TRUE(obj);
}

TEST_F(SimpleObjectTests, test2) {
    JS::RootedObject obj(rt_);
    rs::jsapi::Object::Create(rt_, {"hello"}, nullptr, nullptr, {}, obj);
    ASSERT_TRUE(obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(rt_);
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(rt_, obj, "hello", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
}

TEST_F(SimpleObjectTests, test3) {
    JS::RootedObject obj(rt_);
    rs::jsapi::Object::Create(rt_, {"hello", "pi", "lorem", "the_answer"}, nullptr, nullptr, {}, obj);
    ASSERT_TRUE(obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(rt_);
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(rt_, obj, "hello", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(rt_, obj, "pi", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(rt_, obj, "lorem", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(rt_, obj, "the_answer", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
}

TEST_F(SimpleObjectTests, test4) {
    JS::RootedObject obj(rt_);
    rs::jsapi::Object::Create(rt_, 
        {"hello", "pi", "lorem", "the_answer"}, 
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) {
            if (std::strcmp(name, "hello") == 0) {
                value.setString(JS_NewStringCopyZ(cx, "world"));
            } else if (std::strcmp(name, "pi") == 0) {
                value.setNumber(3.14159);
            } else if (std::strcmp(name, "lorem") == 0) {
                value.setString(JS_NewStringCopyZ(cx, "Lorem ipsum..."));
            } else if (std::strcmp(name, "the_answer") == 0) {
                value.setInt32(42);
            } else {
                value.setUndefined();
            }
            return true;
        }, 
        nullptr, 
        {}, 
        obj);
    ASSERT_TRUE(obj);
    
    rs::jsapi::Result value(rt_);
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "hello", value));
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("world", value.ToString().c_str());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "pi", value));
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "lorem", value));
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("Lorem ipsum...", value.ToString().c_str());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "the_answer", value));
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "xyz", value));
    ASSERT_TRUE(value.isUndefined());
}

TEST_F(SimpleObjectTests, test5) {
    JS::RootedObject obj(rt_);
    rs::jsapi::Object::Create(rt_, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {}, 
        obj);
    ASSERT_TRUE(obj);
    
    rs::jsapi::Result value(rt_);
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "hello", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "pi", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "lorem", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "the_answer", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(rt_, obj, "xyz", value));
    ASSERT_TRUE(value.isUndefined());
}

TEST_F(SimpleObjectTests, test6) {
    std::string longFieldName(384, '1');
    
    JS::RootedObject obj(rt_);
    rs::jsapi::Object::Create(rt_, 
        { longFieldName.c_str() },
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) { value.setInt32(42); return true; },
        nullptr, 
        {}, 
        obj);
    ASSERT_TRUE(obj);
    
    rs::jsapi::Result value(rt_);
    ASSERT_TRUE(JS_GetProperty(rt_, obj, longFieldName.c_str(), value));
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
}