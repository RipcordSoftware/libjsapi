#include "entry.h"

Entry::Entry(rs::jsapi::Runtime& rt, Gtk::Entry* entry) : rt_(rt), obj_(rt), entry_(entry), widget_(entry, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("setText", std::bind(&Entry::SetText, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("getText", std::bind(&Entry::GetText, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&Entry::Finalizer, this), obj_);
}

void Entry::SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    entry_->set_text(args[0].ToString());
    result = *this;
}

void Entry::GetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = entry_->get_text();
}