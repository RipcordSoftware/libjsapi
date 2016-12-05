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

#ifndef CHECK_BUTTON_H
#define	CHECK_BUTTON_H

#include "libjsapi.h"

#include "widget.h"

class CheckButton {
public:
    CheckButton(rs::jsapi::Context& cx, Gtk::CheckButton* button);
    
    void GetActive(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetActive(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void OnClick(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    operator rs::jsapi::Value&() { return obj_; }

private:
    void Finalizer() {
        delete this;
    }
    
    void GetCallback(const char* name, rs::jsapi::Value& value);
    void SetCallback(const char* name, const rs::jsapi::Value& value);        
    
    void OnButtonClicked();
    
    rs::jsapi::Context& cx_;
    rs::jsapi::Value obj_;
    Gtk::CheckButton* button_;    
    Widget widget_; 
    
    rs::jsapi::Value onClick_ { cx_ };

};

#endif	/* CHECK_BUTTON_H */

