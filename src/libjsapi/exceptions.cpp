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

#include "exceptions.h"

#include <map>

static std::map<int16_t, std::string> jsapiExceptionTypes = {
    { JSEXN_INTERNALERR, "InternalError: " },
    { JSEXN_EVALERR, "EvaluationError: " },
    { JSEXN_RANGEERR, "RangeError: " },
    { JSEXN_REFERENCEERR, "ReferenceError: " },    
    { JSEXN_SYNTAXERR, "SyntaxError: " },
    { JSEXN_TYPEERR, "TypeError: " },
    { JSEXN_URIERR, "UriError: " },
    { JSEXN_DEBUGGEEWOULDRUN, "DebuggeeWouldRun: " },
    { JSEXN_WARN, "Warning: " }
};

std::string rs::jsapi::ScriptException::U16toString(const char16_t* ptr) {
    std::string str;
    if (ptr) {
        while (*ptr != '\0') {
            str += *ptr < 128 ? (char) *ptr : '?';
            ++ptr;
        }
    }
    return str;
}

std::string rs::jsapi::ScriptException::GetExceptionMessage(const JSErrorReport* error) {
    std::string str;
    
    if (error && error->ucmessage && jsapiExceptionTypes.find(error->exnType) != jsapiExceptionTypes.cend()) {
        str = jsapiExceptionTypes[error->exnType] + U16toString(error->ucmessage);
    }
    
    return str;
}
