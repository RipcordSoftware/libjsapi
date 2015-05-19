/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ripcord Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

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