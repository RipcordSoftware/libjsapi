#include "entry.h"

#include <cstring>

Entry::Entry(rs::jsapi::Runtime& rt, Gtk::Entry* entry) : rt_(rt), obj_(rt), entry_(entry), widget_(entry, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("setText", std::bind(&Entry::SetText, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("getText", std::bind(&Entry::GetText, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(rt, { "value", "innerHTML" }, 
        std::bind(&Entry::GetCallback, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Entry::SetCallback, this, std::placeholders::_1, std::placeholders::_2),
        functions, std::bind(&Entry::Finalizer, this), obj_);
}

void Entry::GetCallback(const char* name, rs::jsapi::Value& value) {
    if (std::strcmp(name, "value") == 0 || std::strcmp(name, "innerHTML") == 0) {
        GetText({}, value);
    }
}

void Entry::SetCallback(const char* name, const rs::jsapi::Value& value) {        
    if (std::strcmp(name, "value") == 0 || std::strcmp(name, "innerHTML") == 0) {
        std::vector<rs::jsapi::Value> args;
        args.push_back(value);
        rs::jsapi::Value result(rt_);
        SetText(args, result);
    }   
}

void Entry::SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    entry_->set_text(args[0].ToString());
    result = *this;
}

void Entry::GetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = entry_->get_text();
}