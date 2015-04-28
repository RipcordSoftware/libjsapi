#include <gtest/gtest.h>

#include "../libjsapi.h"

#include <cstring>
#include <string>
#include <sstream>

rs::jsapi::Runtime rt_;

class SimpleObjectTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(SimpleObjectTests, test1) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
}

TEST_F(SimpleObjectTests, test2) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {"hello"}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(*context);
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "hello", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
}

TEST_F(SimpleObjectTests, test3) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {"hello", "pi", "lorem", "the_answer"}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(*context);
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "hello", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "pi", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "lorem", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "the_answer", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
}

TEST_F(SimpleObjectTests, test4) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        [](const char* name, rs::jsapi::Value& value) {
            if (std::strcmp(name, "hello") == 0) {
                value.set("world");
            } else if (std::strcmp(name, "pi") == 0) {
                value.set(3.14159);
            } else if (std::strcmp(name, "lorem") == 0) {
                value.set("Lorem ipsum...");
            } else if (std::strcmp(name, "the_answer") == 0) {
                value.set(42);
            } else {
                value.setUndefined();
            }
            return true;
        }, 
        nullptr, 
        {},
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetProperty(*context, obj, "hello", value));
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("world", value.ToString().c_str());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "pi", value));
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "lorem", value));
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("Lorem ipsum...", value.ToString().c_str());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "the_answer", value));
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "xyz", value));
    ASSERT_TRUE(value.isUndefined());
}

TEST_F(SimpleObjectTests, test5) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {},
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetProperty(*context, obj, "hello", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "pi", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "lorem", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "the_answer", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "xyz", value));
    ASSERT_TRUE(value.isUndefined());
}

TEST_F(SimpleObjectTests, test5b) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    ASSERT_TRUE(JS_SetProperty(*context, obj, "hello", rs::jsapi::Value(rt_, "world")));
    ASSERT_TRUE(JS_SetProperty(*context, obj, "pi", rs::jsapi::Value(rt_, 3.14159)));
    ASSERT_TRUE(JS_SetProperty(*context, obj, "the_answer", rs::jsapi::Value(rt_, 42)));
    ASSERT_TRUE(JS_SetProperty(*context, obj, "lorem", rs::jsapi::Value(rt_, "Lorem ipsum...")));
}

TEST_F(SimpleObjectTests, test6) {
    auto context = rt_.NewContext();
    
    std::string longFieldName(384, '1');
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        { longFieldName.c_str() },
        [](const char* name, rs::jsapi::Value& value) { value.set(42); return true; },
        nullptr, 
        {}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetProperty(*context, obj, longFieldName.c_str(), value));
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
}

TEST_F(SimpleObjectTests, test7) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "the_answer" }, 
        [](const char* name, rs::jsapi::Value& value) { value.set(42); return true; },
        nullptr,
        {},
        nullptr, 
        obj);
           
    context->Evaluate("var myfunc=function(n){return n.the_answer;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleObjectTests, test8) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "hello" }, 
        [](const char* name, rs::jsapi::Value& value) { value.set("world"); return true; },
        nullptr,
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(n){return n.hello;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("world", result.ToString().c_str());
}

TEST_F(SimpleObjectTests, test9) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {},
        nullptr,
        nullptr, 
        { std::make_pair("myfunc", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result){} )}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(*context);
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "myfunc", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    ASSERT_TRUE(JS_IsNative(desc.object()));
}

TEST_F(SimpleObjectTests, test10) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {},
        nullptr,
        nullptr, 
        { std::make_pair("myfunc", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
            result.set(42);
        })}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::FunctionArguments args(*context);
    rs::jsapi::Value result(*context);
    ASSERT_TRUE(JS_CallFunctionName(*context, obj, "myfunc", args, result));
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleObjectTests, test11) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {},
        nullptr,
        nullptr, 
        { 
            std::make_pair("myfunc", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
                result.set(42);
            })
        },
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
      
    context->Evaluate("var myfunc = function(o) { return o.myfunc(); }");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);    
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleObjectTests, test12) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value fieldValue(*context);
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "hello" },         
        nullptr,
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue.set(value); 
            return true; 
        },
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(o){ o.hello = 'world';};");    
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    context->Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());    
}

TEST_F(SimpleObjectTests, test13) {
    auto context = rt_.NewContext();
    
    std::string longFieldName(384, '1');
    rs::jsapi::Value fieldValue(*context);
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { longFieldName.c_str() },         
        nullptr,
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue.set(value); 
            return true; 
        },
        {},
        nullptr, 
        obj);
        
    std::stringstream script;
    script << "var myfunc=function(o){ o['" << longFieldName << "'] = 'world';};";
    context->Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    context->Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());    
}

TEST_F(SimpleObjectTests, test14) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {},
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context->Evaluate("var myfunc = function(o){ var arr=[]; for (var prop in o) { arr.push(prop); } return arr.toString(); };");
    
    rs::jsapi::Value result(*context);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello,pi,lorem,the_answer", result.ToString().c_str());    
}

TEST_F(SimpleObjectTests, test15) {    
    auto count = 0;
    
    if (true) {
        auto context = rt_.NewContext();
        
        rs::jsapi::Value obj(*context);
        rs::jsapi::Object::Create(*context, 
            {}, 
            nullptr, 
            nullptr, 
            {},
            [&]() { ++count; },
            obj);
        ASSERT_TRUE(!!obj);
    }
    
    ASSERT_EQ(1, count);
}