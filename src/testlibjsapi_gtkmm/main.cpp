#include <gtkmm.h>

#include <string>
#include <fstream>

#include "libjsapi.h"

#include "application.h"
#include "window.h"
#include "builder.h"

std::string LoadScript(int argc, char** argv) {
    std::string script;
    
    for (int i = 1; i < argc; i++) {
        std::fstream file;
        file.open(argv[i], std::ios::in);
        while (!!file) {
            char buffer[4096];
            file.read(buffer, sizeof(buffer));
            script.append(buffer, file.gcount());
            script += ";\n";
        }
    }
    
    return std::move(script);
}

int main(int argc, char** argv) {  
    
    std::string script = LoadScript(argc, argv);
            
    rs::jsapi::Runtime rt;
    
    auto app = new Application(rt, "com.ripcordsoftware.examples.gtk", 1, argv);
    rs::jsapi::Global::DefineProperty(rt, "app", *app);
    
    auto builder = new Builder(rt);
    rs::jsapi::Global::DefineProperty(rt, "builder", *builder);
    
    rt.Evaluate(script.c_str());
    
    return 0;
}