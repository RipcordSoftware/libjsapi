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

#ifndef WIDGET_H
#define	WIDGET_H

#include <gtkmm.h>

#include "libjsapi.h"

class Widget {
public:
    Widget(Gtk::Widget* widget, rs::jsapi::Value& parent);
    
    void Show(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void Hide(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetHeight(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetOpacity(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetOpacity(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetName(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetName(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    std::vector<std::pair<const char*, rs::jsapi::Object::FunctionCallback>> GetFunctions() {
        return std::vector<std::pair<const char*, rs::jsapi::Object::FunctionCallback>> {
            { "show", std::bind(&Widget::Show, this, std::placeholders::_1, std::placeholders::_2) }, 
            { "hide", std::bind(&Widget::Hide, this, std::placeholders::_1, std::placeholders::_2) }, 
            { "getWidth", std::bind(&Widget::GetWidth, this, std::placeholders::_1, std::placeholders::_2) },
            { "getHeight", std::bind(&Widget::GetHeight, this, std::placeholders::_1, std::placeholders::_2) },
            { "getName", std::bind(&Widget::GetName, this, std::placeholders::_1, std::placeholders::_2) },
            { "setName", std::bind(&Widget::SetName, this, std::placeholders::_1, std::placeholders::_2) },
            { "getOpacity", std::bind(&Widget::GetOpacity, this, std::placeholders::_1, std::placeholders::_2) },
            { "setOpacity", std::bind(&Widget::SetOpacity, this, std::placeholders::_1, std::placeholders::_2) },
        };
    }
    
private:
    Gtk::Widget* widget_;
    rs::jsapi::Value& parent_;
};

#endif	/* WIDGET_H */

