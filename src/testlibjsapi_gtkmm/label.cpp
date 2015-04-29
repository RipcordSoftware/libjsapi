#include "label.h"

Label::Label(rs::jsapi::Runtime& rt, Gtk::Label* label) : rt_(rt), obj_(rt), label_(label), widget_(label, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("setText", std::bind(&Label::SetText, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&Label::Finalizer, this), obj_);
}

void Label::SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    label_->set_text(args[0].ToString());
    result = *this;
}