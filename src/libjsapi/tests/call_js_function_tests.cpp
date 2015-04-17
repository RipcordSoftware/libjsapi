#include <gtest/gtest.h>

#include <thread>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class CallJSFunctionTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }    
};

TEST_F(CallJSFunctionTests, test1) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(){return 42;};");
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", result);
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(CallJSFunctionTests, test2) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc1=function(){return 42;};");
    context->Evaluate("var myfunc2=function(){return 69;};");
    
    rs::jsapi::Result result1(*context);
    context->Call("myfunc1", result1);
    
    ASSERT_TRUE(result1.isNumber());
    ASSERT_EQ(42, result1.toNumber());
    
    rs::jsapi::Result result2(*context);
    context->Call("myfunc2", result2);
    
    ASSERT_TRUE(result2.isNumber());
    ASSERT_EQ(69, result2.toNumber());            
}

TEST_F(CallJSFunctionTests, test3) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Result result1(*context);
    context->Evaluate("var myfunc=function(){return 42;};(function(){return myfunc()+1;})();", result1);    
    
    ASSERT_TRUE(result1.isNumber());
    ASSERT_EQ(43, result1.toNumber());
    
    rs::jsapi::Result result2(*context);
    context->Call("myfunc", result2);            
    
    ASSERT_TRUE(result2.isNumber());
    ASSERT_EQ(42, result2.toNumber());
}

TEST_F(CallJSFunctionTests, test4) {
    auto context = rt_.NewContext();
    
    context->Evaluate("var count=7;var inc=function(){count++;};var get=function(){return count;};");        
    
    rs::jsapi::Result result1(*context);
    context->Call("get", result1);
    
    ASSERT_TRUE(result1.isNumber());
    ASSERT_EQ(7, result1.toNumber());
    
    context->Call("inc");
    
    rs::jsapi::Result result2(*context);
    context->Call("get", result2);            
    
    ASSERT_TRUE(result2.isNumber());
    ASSERT_EQ(8, result2.toNumber());
}

TEST_F(CallJSFunctionTests, test5) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(n){return n;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(3.14159);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_FLOAT_EQ(3.14159, result.toNumber());    
}

TEST_F(CallJSFunctionTests, test5b) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(n){return n;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(3);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(3, result.toInt32());       
}

TEST_F(CallJSFunctionTests, test5c) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(n){return n;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(7u);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(7, result.toNumber());
}

TEST_F(CallJSFunctionTests, test6) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(n){return n;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append("hello");
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);       
    
    ASSERT_TRUE(result.isString());
    ASSERT_STRCASEEQ("hello", result.ToString().c_str());
}

TEST_F(CallJSFunctionTests, test7) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(n){return n;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(true);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);       
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallJSFunctionTests, test8) {
    auto context = rt_.NewContext();
    context->Evaluate("var myfunc=function(n){return n;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(false);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);       
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_FALSE(result.toBoolean());
    
    args.Clear();
    ASSERT_TRUE(args.Empty());
    
    args.Append(true);    
    context->Call("myfunc", args, result);       
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());    
}

TEST_F(CallJSFunctionTests, test9) {
    auto context = rt_.NewContext();    
     
    rs::jsapi::Object obj(*context);
    obj.DefineProperty("the_answer", [](JSContext* cx, unsigned argc, JS::Value* vp) { *vp = JS::Int32Value(42); return true; });
        
    context->Evaluate("var myfunc=function(n){return n.the_answer;};");    
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());    
}

TEST_F(CallJSFunctionTests, test10) {
    auto context = rt_.NewContext();    
     
    rs::jsapi::Object obj(*context);
    obj.DefineProperty("hello", [](JSContext* cx, unsigned argc, JS::Value* vp) { *vp = JS::StringValue(JS_NewStringCopyZ(cx, "world")); return true; });
        
    context->Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("world", result.ToString().c_str());    
}

TEST_F(CallJSFunctionTests, test11) {
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) {            
            value.setInt32(42);
            return true;
        }, 
        nullptr, obj);            
    
    context->Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(42, result.toInt32());
}

TEST_F(CallJSFunctionTests, test12) {
    auto context = rt_.NewContext();    
    
    JS::RootedObject obj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](JSContext* cx, const char* name, JS::MutableHandleValue value) {            
            value.setString(JS_NewStringCopyZ(cx, "world"));
            return true;
        }, 
        nullptr, obj);            
    
    context->Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(*context);    
    args.Append(obj);
    
    rs::jsapi::Result result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("world", result.ToString().c_str());
}