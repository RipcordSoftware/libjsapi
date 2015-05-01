#include <gtest/gtest.h>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class GlobalPropertyTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(GlobalPropertyTests, test1) {
    ASSERT_TRUE(rs::jsapi::Global::DefineProperty(rt_, "test1", 
        [](JSContext* cx, unsigned argc, JS::Value* vp) {
            auto args = JS::CallArgsFromVp(argc, vp);
            args.rval().setInt32(42);
            return true;
        },
        nullptr)
    );
        
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return test1;})();", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(GlobalPropertyTests, test2) {
    ASSERT_TRUE(rs::jsapi::Global::DefineProperty(rt_, "test2", 
        [](JSContext* cx, unsigned argc, JS::Value* vp) {
            auto args = JS::CallArgsFromVp(argc, vp);
            args.rval().setString(JS_NewStringCopyZ(cx, "hello"));
            return true;
        },
        nullptr)
    );
        
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return test2;})();", result);
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(GlobalPropertyTests, test3) {
    ASSERT_TRUE(rs::jsapi::Global::DefineProperty(rt_, "test3", 
        [](JSContext* cx, unsigned argc, JS::Value* vp) {
            auto args = JS::CallArgsFromVp(argc, vp);
            args.rval().setObject(*JS_NewObject(cx, nullptr, JS::NullPtr(), JS::NullPtr()));
            return true;
        },
        nullptr)
    );
        
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return test3;})();", result);
    ASSERT_TRUE(result.isObject());
}

TEST_F(GlobalPropertyTests, test4) {
    rs::jsapi::Value value(rt_, 99);
    rs::jsapi::Global::DefineProperty(rt_, "test4", value); 
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return test4;})();", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(99, result.toInt32());
}

TEST_F(GlobalPropertyTests, test5) {
    rs::jsapi::Value obj(rt_);
    rs::jsapi::Object::Create(rt_, 
        {}, 
        nullptr, 
        nullptr, 
        {},
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Global::DefineProperty(rt_, "test5", obj); 
        
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return test5;})();", result);
    ASSERT_TRUE(result.isObject());
}