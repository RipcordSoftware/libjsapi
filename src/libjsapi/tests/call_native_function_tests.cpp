#include <gtest/gtest.h>

#include <thread>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class CallNativeFunctionTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
public:
    static bool GetTheAnswer(JSContext* cx, unsigned argc, JS::Value* vp);
    static bool GetLorem(JSContext* cx, unsigned argc, JS::Value* vp);
    static bool Echo(JSContext* cx, unsigned argc, JS::Value* vp);
};

bool CallNativeFunctionTests::GetTheAnswer(JSContext* cx, unsigned argc, JS::Value* vp) {
    auto args = JS::CallArgsFromVp(argc, vp);
    args.rval().setInt32(42);
    return true;
}

bool CallNativeFunctionTests::GetLorem(JSContext* cx, unsigned argc, JS::Value* vp) {
    auto args = JS::CallArgsFromVp(argc, vp);

    JS::RootedString str(cx, JS_NewStringCopyZ(cx, "Lorem ipsum"));
    args.rval().setString(str);
    return true;
}

bool CallNativeFunctionTests::Echo(JSContext* cx, unsigned argc, JS::Value* vp) {
    auto args = JS::CallArgsFromVp(argc, vp);

    if (argc > 0) {
        auto value = args.get(0);
        args.rval().set(value);
    } else {    
        args.rval().setNull();
    }
    return true;
}

TEST_F(CallNativeFunctionTests, test1) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "getTheAnswer", &CallNativeFunctionTests::GetTheAnswer, 0, 0);
    
    rs::jsapi::Result result(*context);
    context->Evaluate("(function(){return getTheAnswer();})();", result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());    
}

TEST_F(CallNativeFunctionTests, test2) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "getLorem", &CallNativeFunctionTests::GetLorem, 0, 0);
    
    rs::jsapi::Result result(*context);
    context->Evaluate("(function(){return getLorem();})();", result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("Lorem ipsum", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test3) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(true);
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result); 
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallNativeFunctionTests, test4) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(42);
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result); 
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(CallNativeFunctionTests, test5) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append("hello");
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result); 
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test6) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::Value value(*context, "hello");
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test7) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::Value value(*context, true);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallNativeFunctionTests, test8) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::Value value(*context, 42);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_TRUE(result.toInt32());
}

TEST_F(CallNativeFunctionTests, test9) {
    auto context = rt_.NewContext();
    
    JS_DefineFunction(*context, context->getGlobal(), "echo", &CallNativeFunctionTests::Echo, 1, 0);
    
    rs::jsapi::Value value(*context, 3.14159);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Result result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_FLOAT_EQ(3.14159, result.toNumber());
}