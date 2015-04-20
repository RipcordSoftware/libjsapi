#include <gtest/gtest.h>

#include <thread>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class SimpleDynamicObjectTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }    
};

TEST_F(SimpleDynamicObjectTests, test1) {
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) {            
            value.setInt32(42);
            return true;
        }, 
        nullptr, obj);
    ASSERT_TRUE(obj);
    
    context->Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(42, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test2) {
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) {            
            value.setString(JS_NewStringCopyZ(cx, "world"));
            return true;
        }, 
        nullptr, obj);            
    ASSERT_TRUE(obj);
    
    context->Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("world", result.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test3) {
    std::string longFieldName(384, '1');
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) {            
            value.setInt32(42);
            return true;
        }, 
        nullptr, obj);
    ASSERT_TRUE(obj);
    
    std::stringstream script;
    script << "var myfunc=function(o){ return o['" << longFieldName << "'];};";
    context->Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(42, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test4) {
    rs::jsapi::Value fieldValue(rt_);
    
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        [&](JSContext* cx, const char* name, JS::MutableHandleValue value) { 
            fieldValue.set(value); 
            return true; 
        },
        obj);        
    ASSERT_TRUE(obj);
        
    rt_.Evaluate("var myfunc=function(o){ o.test = 'world';};");
    
    rs::jsapi::FunctionArguments args(rt_);
    args.Append(obj);
    
    rt_.Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test5) {
    std::string longFieldName(384, '1');
    rs::jsapi::Value fieldValue(rt_);
    
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        [&](JSContext* cx, const char* name, JS::MutableHandleValue value) { 
            fieldValue.set(value); 
            return true; 
        },
        obj);
    ASSERT_TRUE(obj);
    
    std::stringstream script;
    script << "var myfunc=function(o){ o['" << longFieldName << "'] = 'world';};";
    rt_.Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(rt_);
    args.Append(obj);
    
    rt_.Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());
}