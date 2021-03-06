/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Ripcord Software Ltd
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

#ifndef RS_JSAPI_EXCEPTIONS_H
#define RS_JSAPI_EXCEPTIONS_H

#include <exception>
#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class Exception : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "An exception in JSAPI occurred";
    }
};

class ContextWrongThreadException : public Exception {
public:
    virtual const char* what() const noexcept override {
        return "Context call made from the wrong thread";
    }
};

class ContextThreadInstanceException : public Exception {
public:
    virtual const char* what() const noexcept override {
        return "A context is already active on this thread";
    }
};

class ScriptException : public Exception {
public:
    ScriptException(const char* message, const JSErrorReport* error) :
        message(message != nullptr ? message : ""),
        filename(error->filename != nullptr ? error->filename : ""), 
        lineno(error->lineno),
        column(error->column), 
        uclinebuf(error->linebuf() != nullptr ? error->linebuf() : u""),
        uctokenOffset(error->tokenOffset()),
        errorNumber(error->errorNumber),
        exnType(error->exnType) {
    }
        
    ScriptException(const JSErrorReport* error) : 
        ScriptException(GetExceptionMessage(error).c_str(), error) {}
        
    virtual const char* what() const noexcept override {
        return message.length() > 0 ? message.c_str() : "An error or exception happened in JSAPI";
    }
    
    const std::string message;
    const std::string filename;
    const unsigned lineno;
    const unsigned column;
    const std::u16string uclinebuf;
    const unsigned uctokenOffset;
    const unsigned errorNumber;
    const int16_t exnType;
    
private:
    static std::string U16toString(const char16_t* ptr);
    static std::string GetExceptionMessage(const JSErrorReport* error);
};

class ValueCastException : public Exception {
public:
    virtual const char* what() const noexcept override {
        return "Invalid object or value cast";
    }    
};

class FunctionArgumentsIndexException : public Exception {
public:
    virtual const char* what() const noexcept override {
        return "The FunctionArguments index value was invalid";
    }    
};
    
}}
    
#endif	/* RS_JSAPI_EXCEPTIONS_H */

