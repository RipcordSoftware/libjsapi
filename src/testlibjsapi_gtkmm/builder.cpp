#include "builder.h"

#include <exception>
#include <gtk-3.0/gtk/gtkbutton.h>

#include "window.h"
#include "button.h"

class BuilderException : public std::exception {
public:
    BuilderException(const char* msg) : msg_(msg) {}
    
    virtual const char* what() const noexcept override { return msg_.c_str(); } 
    
private:
    std::string msg_;
};

Builder::Builder(rs::jsapi::Runtime& rt) : rt_(rt), obj_(rt), null_(rt) {
    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, {
        { "addFromFile", std::bind(&Builder::AddFromFile, this, std::placeholders::_1, std::placeholders::_2) },
        { "getWindow", std::bind(&Builder::GetWindow, this, std::placeholders::_1, std::placeholders::_2) },
        { "getButton", std::bind(&Builder::GetButton, this, std::placeholders::_1, std::placeholders::_2) }
    }, nullptr, obj_);
}

void Builder::AddFromFile(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (args.size() > 0 && args[0].isString()) {
        builder_ = Gtk::Builder::create();
        
        try {
            builder_->add_from_file(args[0].ToString());             
            result = *this;
        } catch (const Glib::Exception& ex) {
            throw BuilderException(ex.what().c_str());
        }
    }    
}

void Builder::GetWindow(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = GetWidget(args);
    if (widget && GTK_IS_WINDOW(widget->gobj())) {
        auto window = reinterpret_cast<Gtk::Window*>(widget);
        result = *(new Window(rt_, window));
    }
}

void Builder::GetButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = GetWidget(args);
    if (widget && GTK_IS_BUTTON(widget->gobj())) {
        auto button = reinterpret_cast<Gtk::Button*>(widget);
        result = *(new Button(rt_, button));
    }
}

Gtk::Widget* Builder::GetWidget(const std::vector<rs::jsapi::Value>& args) {
    Gtk::Widget* widget = nullptr;
    if (args.size() > 0 && args[0].isString()) {        
        builder_->get_widget(args[0].ToString(), widget);
    }
    return widget;
}