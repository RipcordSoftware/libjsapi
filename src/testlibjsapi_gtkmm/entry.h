#ifndef ENTRY_H
#define	ENTRY_H

#include "libjsapi.h"

#include "widget.h"

class Entry {
public:
    Entry(rs::jsapi::Runtime& rt, Gtk::Entry* entry);
    
    void GetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    void Finalizer() {
        delete this;
    }
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::Entry* entry_;    
    Widget widget_; 

};

#endif	/* ENTRY_H */

