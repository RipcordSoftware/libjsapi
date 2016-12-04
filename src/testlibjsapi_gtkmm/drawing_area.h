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

#ifndef DRAWING_AREA_H
#define	DRAWING_AREA_H

#include "libjsapi.h"

#include "widget.h"

class DrawingArea {
public:
    DrawingArea(rs::jsapi::Context& cx, Gtk::DrawingArea* entry);
    
    void SetSourceRgb(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetLineWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void MoveTo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void LineTo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void Stroke(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void OnDraw(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);        
    void CreateImageSurface(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void CreateImageSurfaceFromPng(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void PutImageData(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    void Finalizer() {
        delete this;
    }
    
    bool OnGtkDraw(const Cairo::RefPtr<Cairo::Context>& cr);
    
    rs::jsapi::Context& cx_;
    rs::jsapi::Value obj_;
    Gtk::DrawingArea* area_;    
    Widget widget_; 
    
    Cairo::RefPtr<Cairo::Context> onDrawContext_;
    rs::jsapi::Value onDraw_ { cx_ };
    
    Cairo::RefPtr<Cairo::ImageSurface> image_;
};

#endif	/* DRAWING_AREA_H */

