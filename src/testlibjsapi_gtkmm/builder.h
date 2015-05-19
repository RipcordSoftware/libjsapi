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

#ifndef BUILDER_H
#define	BUILDER_H

#include <gtkmm.h>

#include "libjsapi.h"

class Builder {
public:
    Builder(rs::jsapi::Runtime& rt);
    
    void AddFromFile(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetWidget(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetWindow(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetCheckButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetEntry(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void GetDrawingArea(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    Gtk::Widget* FindWidget(const std::vector<rs::jsapi::Value>& args);
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Glib::RefPtr<Gtk::Builder> builder_;
    
    rs::jsapi::Value null_;
};

#endif	/* BUILDER_H */

