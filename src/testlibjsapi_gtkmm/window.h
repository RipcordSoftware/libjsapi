#ifndef WINDOW_H
#define	WINDOW_H

#include <gtkmm.h>

#include "libjsapi.h"

#include "widget.h"

class Window {
public:
    Window(rs::jsapi::Runtime& rt, Gtk::Window* window);        
    
    void SetDefaultSize(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void SetTitle(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void SetBorderWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void AddLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void AddButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    operator Gtk::Window&() { return *window_; }
    operator rs::jsapi::Value&() { return obj_; }
    
    static Gtk::Window* getWindowFromValue(const rs::jsapi::Value&);
    
private:
    void Finalizer() {
        delete this;
    }
    
    void GetCallback(const char* name, rs::jsapi::Value& value);
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::Window* window_;  
    Widget widget_;
};

#endif	/* WINDOW_H */
