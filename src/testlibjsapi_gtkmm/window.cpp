#include "window.h"

#include <typeinfo>
#include <cstring>

#include "widget.h"
#include "label.h"
#include "button.h"

Window::Window(rs::jsapi::Runtime& rt, Gtk::Window* window) : rt_(rt), obj_(rt), widget_(window_, obj_), window_(window) {
    
    auto functions = widget_.GetFunctions();
    
    functions.emplace_back("setDefaultSize", std::bind(&Window::SetDefaultSize, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("setTitle", std::bind(&Window::SetTitle, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("setBorderWidth", std::bind(&Window::SetBorderWidth, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("getLabel", std::bind(&Window::GetLabel, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("addLabel", std::bind(&Window::AddLabel, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("addButton", std::bind(&Window::AddButton, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(rt, 
        { "width", "height" }, 
        std::bind(&Window::GetCallback, this, std::placeholders::_1, std::placeholders::_2),
        nullptr,
        functions, std::bind(&Window::Finalizer, this), obj_);
    
    rs::jsapi::Object::SetPrivate(obj_, typeid(Window).hash_code(), this);
}

bool Window::GetCallback(const char* name, rs::jsapi::Value& value) {
    auto status = false;
    
    if (std::strcmp(name, "width") == 0) {
        value = window_->get_width();
        status = true;
    } else if (std::strcmp(name, "height") == 0) {
        value = window_->get_height();
        status = true;
    }   
    
    return status;
}

void Window::SetDefaultSize(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    auto width = args[0].toInt32();
    auto height = args[1].toInt32();
    window_->set_default_size(width, height);
    result = *this;
}

void Window::SetTitle(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    window_->set_title(args[0].ToString());
    result = *this;
}

void Window::SetBorderWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    window_->set_border_width(args[0].toInt32());
    result = *this;
}

void Window::GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    auto name = args[0].ToString();
    auto children = window_->get_children();

    for (auto c : children) {
        if (c->get_name().compare(name) == 0) {
            auto label = new Label(rt_, reinterpret_cast<Gtk::Label*>(children[0]));
            result = *label;
            return;
        }
    }

    result.setNull();        
}       

void Window::AddLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    auto label = Gtk::manage(new Gtk::Label());

    if (args.size() > 0 && args[0].isString()) {
        label->set_name(args[0].ToString());
    }

    window_->add(*label);
    result = *(new Label(rt_, label));
}      

void Window::AddButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    auto button = Gtk::manage(new Gtk::Button());

    if (args.size() > 0 && args[0].isString()) {
        button->set_name(args[0].ToString());
    }

    window_->add(*button);
    result = *(new Button(rt_, button));
}  

Gtk::Window* Window::getWindowFromValue(const rs::jsapi::Value& value) {
    Gtk::Window* window = nullptr;
    
    uint64_t data = 0;
    void* ptr = nullptr;
    if (rs::jsapi::Object::GetPrivate(value, data, ptr) && data == typeid(Window).hash_code()) {
        auto that = reinterpret_cast<Window*>(ptr);
        window = that->window_;
    }
    
    return window;
}