#include <gtest/gtest.h>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class CallNativeFunctionTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
    static bool Echo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
};

class CallNativeFunctionTestException : public std::exception {
public:
    CallNativeFunctionTestException(const char* msg) : msg_(msg) {
    }
    
    const char* what() const throw() override { return msg_.c_str(); }
    
private:
    const std::string msg_;
};

bool CallNativeFunctionTests::Echo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (args.size() > 0) {
        result = args[0];
    } else {    
        result = JS::NullHandleValue;
    }
    return true;
}

TEST_F(CallNativeFunctionTests, test1) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "getTheAnswer", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { result = 42; });
    
    rs::jsapi::Value result(*context);
    context->Evaluate("(function(){return getTheAnswer();})();", result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());    
}

TEST_F(CallNativeFunctionTests, test1b) {
    auto context = rt_.NewContext();
    int valueClosure = 42;
    
    rs::jsapi::Global::DefineFunction(*context, "getTheAnswer", [&](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { result = valueClosure++; });
    
    rs::jsapi::Value result(*context);
    context->Call("getTheAnswer", result);    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());    
    
    context->Call("getTheAnswer", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(43, result.toInt32());    
    
    context->Call("getTheAnswer", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(44, result.toInt32());
}

TEST_F(CallNativeFunctionTests, test2) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "getLorem", 
        [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { result = "Lorem ipsum"; });
    
    rs::jsapi::Value result(*context);
    context->Evaluate("(function(){return getLorem();})();", result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("Lorem ipsum", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test3) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(true);
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result); 
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallNativeFunctionTests, test4) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(42);
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result); 
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(CallNativeFunctionTests, test5) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append("hello");
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result); 
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test6) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(*context, "hello");
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test7) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(*context, true);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallNativeFunctionTests, test8) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(*context, 42);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_TRUE(result.toInt32());
}

TEST_F(CallNativeFunctionTests, test9) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(*context, 3.14159);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(value);
    
    rs::jsapi::Value result(*context);
    context->Call("echo", args, result);     
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_FLOAT_EQ(3.14159, result.toNumber());
}

TEST_F(CallNativeFunctionTests, test10) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "makeMeSad", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
        throw CallNativeFunctionTestException("It happened!");
    });
    
    ASSERT_THROW({
        rs::jsapi::Value result(*context);
        context->Evaluate("(function(){return makeMeSad();})();", result);
    }, rs::jsapi::ScriptException);
}

TEST_F(CallNativeFunctionTests, test11) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Global::DefineFunction(*context, "makeMeSad", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
        throw CallNativeFunctionTestException("It happened!");
    });
    
    rs::jsapi::Value result(*context);
    context->Evaluate("(function(){ try { return makeMeSad(); } catch (e) { return e.message; } })();", result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("It happened!", result.ToString().c_str());
}