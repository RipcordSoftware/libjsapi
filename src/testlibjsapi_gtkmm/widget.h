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

