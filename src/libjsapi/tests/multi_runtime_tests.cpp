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

#include <thread>

#include "../libjsapi.h"

class MultiRuntimeTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
    rs::jsapi::Runtime rt_;
    
public:
    static bool SanityCheckWhatMessage(const char* msg);
};

bool MultiRuntimeTests::SanityCheckWhatMessage(const char* msg) {
    while (*msg != '\0') {
        if (*msg < 0x0d || *msg > 0x7e) {
            return false;
        }
        msg++;
    }
    return true;
}

TEST_F(MultiRuntimeTests, test1) {
#ifndef __APPLE__
    ASSERT_THROW({
        rs::jsapi::Runtime rt;
    }, rs::jsapi::RuntimeThreadInstanceException);
#endif
}

TEST_F(MultiRuntimeTests, test2) {
    std::thread t([](){
        rs::jsapi::Runtime rt;
        rs::jsapi::Script script(rt, "(function(){return 42;})();");
        script.Compile();

        rs::jsapi::Value result(rt);
        script.Execute(result);

        ASSERT_TRUE(result.isNumber());
        ASSERT_EQ(42, result.toNumber());
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test3) {
    std::thread t([](){
        rs::jsapi::Runtime rt1;
    
        ASSERT_THROW({
            rs::jsapi::Runtime rt2;
        }, rs::jsapi::RuntimeThreadInstanceException);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test4) {
    std::thread t([](){
        rs::jsapi::Runtime rt1;
    
        ASSERT_THROW({
            rs::jsapi::Runtime rt2;
        }, rs::jsapi::RuntimeThreadInstanceException);
        
        ASSERT_THROW({
            rs::jsapi::Runtime rt3;
        }, rs::jsapi::RuntimeThreadInstanceException);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test4b) {
    std::thread t([](){
        bool thrown = false;
        rs::jsapi::Runtime rt1;
    
        try {
            rs::jsapi::Runtime rt2;
        } catch (const rs::jsapi::RuntimeThreadInstanceException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiRuntimeTests::SanityCheckWhatMessage(what));
            thrown = true;
        }
        
        ASSERT_TRUE(thrown);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test5) {
    std::thread t([](){
        do {
            rs::jsapi::Runtime rt;
            rs::jsapi::Script script(rt, "(function(){return 42;})();");
            script.Compile();

            rs::jsapi::Value result(rt);
            script.Execute(result);

            ASSERT_TRUE(result.isNumber());
            ASSERT_EQ(42, result.toNumber());
        } while (false);
        
        do {
            rs::jsapi::Runtime rt;
            rs::jsapi::Script script(rt, "(function(){return 42;})();");
            script.Compile();

            rs::jsapi::Value result(rt);
            script.Execute(result);

            ASSERT_TRUE(result.isNumber());
            ASSERT_EQ(42, result.toNumber());
        } while (false);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test6) {
    std::thread t([](){
        rs::jsapi::Runtime rt;
        
        rs::jsapi::Script bootstrapScript1(rt, "var count = 42;");
        bootstrapScript1.Compile();
        bootstrapScript1.Execute();

        auto context2 = rt.NewContext();
        rs::jsapi::Script bootstrapScript2(*context2, "var count = 42;");
        bootstrapScript2.Compile();
        bootstrapScript2.Execute();

        rs::jsapi::Script script1(rt, "(function(){return count++;})();");
        script1.Compile();

        rs::jsapi::Script script2(*context2, "(function(){return count++;})();");
        script2.Compile();

        rs::jsapi::Value result1a(rt);
        script1.Execute(result1a);

        ASSERT_TRUE(result1a.isNumber());
        ASSERT_EQ(42, result1a.toNumber());

        rs::jsapi::Value result1b(rt);
        script1.Execute(result1b);

        ASSERT_TRUE(result1b.isNumber());
        ASSERT_EQ(43, result1b.toNumber());

        rs::jsapi::Value result2a(*context2);
        script2.Execute(result2a);

        ASSERT_TRUE(result2a.isNumber());
        ASSERT_EQ(42, result2a.toNumber());
    });
    
    t.join();
}