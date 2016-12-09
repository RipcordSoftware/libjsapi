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

class MultiContextTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
    rs::jsapi::Context context_;
    
public:
    static bool SanityCheckWhatMessage(const char* msg);
};

bool MultiContextTests::SanityCheckWhatMessage(const char* msg) {
    while (*msg != '\0') {
        if (*msg < 0x0d || *msg > 0x7e) {
            return false;
        }
        msg++;
    }
    return true;
}

TEST_F(MultiContextTests, test1) {
    std::thread thread([] {
        rs::jsapi::Context cx;
        rs::jsapi::Script script(cx, "(function(){return 42;})();");
        script.Compile();

        rs::jsapi::Value result(cx);
        script.Execute(result);

        ASSERT_TRUE(result.isNumber());
        ASSERT_EQ(42, result.toNumber());
    });
    
    thread.join();
}

TEST_F(MultiContextTests, test2) {
    std::thread t1([] {
        rs::jsapi::Context cx1;
        rs::jsapi::Script bootstrapScript1(cx1, "var count = 42;");
        bootstrapScript1.Compile();
        bootstrapScript1.Execute();

        rs::jsapi::Script script1(cx1, "(function(){return count++;})();");
        script1.Compile();

        rs::jsapi::Value result1a(cx1);
        script1.Execute(result1a);
        
        ASSERT_TRUE(result1a.isNumber());
        ASSERT_EQ(42, result1a.toNumber());

        rs::jsapi::Value result1b(cx1);
        script1.Execute(result1b);

        ASSERT_TRUE(result1b.isNumber());
        ASSERT_EQ(43, result1b.toNumber());
    });
    
    std::thread t2([] {
        rs::jsapi::Context cx2;
        rs::jsapi::Script bootstrapScript2(cx2, "var count = 42;");
        bootstrapScript2.Compile();
        bootstrapScript2.Execute();  

        rs::jsapi::Script script2(cx2, "(function(){return count++;})();");
        script2.Compile();   

        rs::jsapi::Value result2a(cx2);
        script2.Execute(result2a);

        ASSERT_TRUE(result2a.isNumber());
        ASSERT_EQ(42, result2a.toNumber());
    });
    
    t1.join();
    t2.join();
}

TEST_F(MultiContextTests, test3) {
    ASSERT_THROW({
        rs::jsapi::Context cx;
    }, rs::jsapi::ContextThreadInstanceException);
}

TEST_F(MultiContextTests, test4) {        
    std::thread t([&]() {
        bool thrown = false;
        try {
            context_.Evaluate("");
        } catch (const rs::jsapi::ContextWrongThreadException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiContextTests::SanityCheckWhatMessage(what));
            thrown = true;
        }

        ASSERT_TRUE(thrown);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test5) {        
    std::thread t([&]() {
        bool thrown = false;
        try {
            rs::jsapi::Value v{context_};
            context_.Evaluate("", v);
        } catch (const rs::jsapi::ContextWrongThreadException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiContextTests::SanityCheckWhatMessage(what));
            thrown = true;
        }
        
        ASSERT_TRUE(thrown);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test6) {        
    std::thread t([&]() {
        bool thrown = false;
        try {
            context_.Call("");
        } catch (const rs::jsapi::ContextWrongThreadException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiContextTests::SanityCheckWhatMessage(what));
            thrown = true;
        }

        ASSERT_TRUE(thrown);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test7) {
    std::thread t([&]() {
        bool thrown = false;
        try {
            rs::jsapi::Value v{context_};
            context_.Call("", v);
        } catch (const rs::jsapi::ContextWrongThreadException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiContextTests::SanityCheckWhatMessage(what));
            thrown = true;
        }

        ASSERT_TRUE(thrown);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test8) {
    std::thread t([&]() {
        bool thrown = false;
        try {
            rs::jsapi::Value v{context_};
            rs::jsapi::Context::Call(v, {context_});
        } catch (const rs::jsapi::ContextWrongThreadException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiContextTests::SanityCheckWhatMessage(what));
            thrown = true;
        }

        ASSERT_TRUE(thrown);
    });

    t.join();
}

TEST_F(MultiContextTests, test9) {
    std::thread t([&]() {
        bool thrown = false;
        try {
            rs::jsapi::Value v{context_};
            rs::jsapi::Value res{context_};
            rs::jsapi::Context::Call(v, {context_}, res);
        } catch (const rs::jsapi::ContextWrongThreadException& ex) {
            auto what = ex.what();
            ASSERT_TRUE(MultiContextTests::SanityCheckWhatMessage(what));
            thrown = true;
        }

        ASSERT_TRUE(thrown);
    });
       
    t.join();
}