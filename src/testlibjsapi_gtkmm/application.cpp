#include "application.h"

#include "window.h"

Application::Application(rs::jsapi::Runtime& rt, const char* appName, int argc, char** argv) : rt_(rt), obj_(rt) {
    app_ = Gtk::Application::create(argc, argv, appName);
    
    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        { { "run", std::bind(&Application::Run, this, std::placeholders::_1, std::placeholders::_2) } }, 
        std::bind(&Application::Finalizer, this), obj_);
}

void Application::Run(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (args.size() > 0 && args[0].isObject()) {
        auto window = Window::getWindowFromValue(args[0]);
        if (window != nullptr) {
            app_->run(*window);
            result = true;
        }
    }
}