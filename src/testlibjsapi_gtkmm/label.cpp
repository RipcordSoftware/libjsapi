#include "label.h"

#include <cstring>

Label::Label(rs::jsapi::Context& cx, Gtk::Label* label) : cx_(cx), obj_(cx), label_(label), widget_(label, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("setText", std::bind(&Label::SetText, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("getText", std::bind(&Label::GetText, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(cx, { "value", "innerHTML" }, 
        std::bind(&Label::GetCallback, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Label::SetCallback, this, std::placeholders::_1, std::placeholders::_2),
        functions, std::bind(&Label::Finalizer, this), obj_);
}

void Label::GetCallback(const char* name, rs::jsapi::Value& value) {
    if (std::strcmp(name, "value") == 0 || std::strcmp(name, "innerHTML") == 0) {
        GetText({}, value);
    }
}

void Label::SetCallback(const char* name, const rs::jsapi::Value& value) {
    if (std::strcmp(name, "value") == 0 || std::strcmp(name, "innerHTML") == 0) {
        std::vector<rs::jsapi::Value> args;
        args.push_back(value);
        rs::jsapi::Value result(cx_);
        SetText(args, result);
    }
}

void Label::SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    label_->set_text(args[0].ToString());
    result = *this;
}

void Label::GetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = label_->get_text();    
}