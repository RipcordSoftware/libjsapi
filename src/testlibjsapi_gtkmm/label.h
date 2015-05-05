#ifndef LABEL_H
#define	LABEL_H

#include "libjsapi.h"

#include "widget.h"

class Label {
public:

    Label(rs::jsapi::Runtime& rt, Gtk::Label* label);
    
    void GetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    bool GetCallback(const char* name, rs::jsapi::Value& value);
    bool SetCallback(const char* name, const rs::jsapi::Value& value);        
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    void Finalizer() {
        delete this;
    }
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::Label* label_;    
    Widget widget_;    
};

#endif	/* LABEL_H */

