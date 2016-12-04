#include "check_button.h"

#include <cstring>

CheckButton::CheckButton(rs::jsapi::Context& cx, Gtk::CheckButton* button) : cx_(cx), button_(button), obj_(cx), widget_(button, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("getActive", std::bind(&CheckButton::GetActive, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("setActive", std::bind(&CheckButton::SetActive, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("getLabel", std::bind(&CheckButton::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("setLabel", std::bind(&CheckButton::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("onClick", std::bind(&CheckButton::OnClick, this, std::placeholders::_1, std::placeholders::_2)); 

    rs::jsapi::Object::Create(cx, { "checked" }, 
        std::bind(&CheckButton::GetCallback, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&CheckButton::SetCallback, this, std::placeholders::_1, std::placeholders::_2),
        functions, std::bind(&CheckButton::Finalizer, this), obj_);
}

void CheckButton::GetCallback(const char* name, rs::jsapi::Value& value) {
    if (std::strcmp(name, "checked") == 0) {
        GetActive({}, value);
    }
}

void CheckButton::SetCallback(const char* name, const rs::jsapi::Value& value) {
    if (std::strcmp(name, "checked") == 0) {
        std::vector<rs::jsapi::Value> args;
        args.push_back(value);
        rs::jsapi::Value result(cx_);
        SetActive(args, result);
    }    
}

void CheckButton::GetActive(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = button_->get_active();
}

void CheckButton::SetActive(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        button_->set_active(args[0].toBoolean());
    }
    result = *this;
}

void CheckButton::GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = button_->get_label();
}

void CheckButton::SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        button_->set_label(args[0].ToString());
    }
    result = *this;
}

void CheckButton::OnClick(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        if (args[0].isFunction()) {
            onClick_ = args[0].toFunction();
            button_->signal_clicked().connect(sigc::mem_fun(*this, &CheckButton::OnButtonClicked));
        } else {
            onClick_.setUndefined();
        }
    }
    result = *this;
}

void CheckButton::OnButtonClicked() {
    if (onClick_.isFunction()) {   
        rs::jsapi::FunctionArguments args(cx_);
        rs::jsapi::Value result(cx_);
        onClick_.CallFunction(args, result);
    }
}