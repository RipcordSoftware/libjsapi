#include <gtest/gtest.h>

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
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](const char* name, rs::jsapi::Value& value) {            
            value.set(42);
            return true;
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
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
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](const char* name, rs::jsapi::Value& value) {            
            value.set("world");
            return true;
        }, 
        nullptr,
        nullptr, 
        nullptr,
        obj);            
    ASSERT_TRUE(!!obj);
    
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
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](const char* name, rs::jsapi::Value& value) {            
            value.set(42);
            return true;
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
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
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue.set(value); 
            return true; 
        },
        nullptr,
        nullptr,
        obj);        
    ASSERT_TRUE(!!obj);
        
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
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue.set(value); 
            return true; 
        },
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    std::stringstream script;
    script << "var myfunc=function(o){ o['" << longFieldName << "'] = 'world';};";
    rt_.Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(rt_);
    args.Append(obj);
    
    rt_.Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test6) {
    auto context = rt_.NewContext();    
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        nullptr, 
        [](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs) {
            props.push_back("the_answer");
            props.push_back("pi");
            props.push_back("hello");
            return true;
        },
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context->Evaluate("var myfunc=function(o){var fields = 0; for (var prop in o) { ++fields; } return fields;};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(3, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test7) {
    auto context = rt_.NewContext();    
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        nullptr, 
        [](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs) {
            props.push_back("the_answer");
            props.push_back("pi");
            funcs.push_back(std::make_pair("square", nullptr));
            funcs.push_back(std::make_pair("echo", nullptr));
            return true;
        },
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context->Evaluate("var myfunc=function(o){var fields = 0; for (var prop in o) { ++fields; } return fields;};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(4, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test8) {
    auto context = rt_.NewContext();    
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        nullptr, 
        nullptr, 
        [](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs) {
            props.push_back("the_answer");
            props.push_back("pi");
            funcs.push_back(std::make_pair("square", nullptr));
            funcs.push_back(std::make_pair("echo", nullptr));
            return true;
        },
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context->Evaluate("var myfunc=function(o){var fields = []; for (var prop in o) { fields.push(prop); } return fields.toString();};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("the_answer,pi,square,echo", result.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test9) {    
    auto count = 0;
    
    if (true) {
        auto context = rt_.NewContext();
        
        rs::jsapi::Value obj(*context);
        rs::jsapi::DynamicObject::Create(
            *context, 
            nullptr, 
            nullptr, 
            nullptr,
            [&]() { ++count; },
            obj);
        ASSERT_TRUE(!!obj);
    }
    
    ASSERT_EQ(1, count);
}

TEST_F(SimpleDynamicObjectTests, test10) {    
    auto context = rt_.NewContext();

    rs::jsapi::Value obj(*context);
    rs::jsapi::DynamicObject::Create(*context, 
        nullptr, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::DynamicObject::SetPrivate(obj, 123456789, this);
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_TRUE(rs::jsapi::DynamicObject::GetPrivate(obj, data, that));
    ASSERT_EQ(123456789, data);
    ASSERT_EQ(this, that);
}

TEST_F(SimpleDynamicObjectTests, test11) {    
    auto context = rt_.NewContext();

    rs::jsapi::Value obj(*context);
    ASSERT_FALSE(rs::jsapi::DynamicObject::SetPrivate(obj, 123456789, this));
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_FALSE(rs::jsapi::DynamicObject::GetPrivate(obj, data, that));
    ASSERT_NE(123456789, data);
    ASSERT_NE(this, that);
}