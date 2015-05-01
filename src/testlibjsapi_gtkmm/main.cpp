#include <gtkmm.h>

#include <string>
#include <fstream>

#include "libjsapi.h"

#include "application.h"
#include "window.h"
#include "builder.h"

int main(int argc, char** argv) {  
    
    std::string script;
    std::fstream file;
    file.open("test.js", std::ios::in);
    while (!!file) {
        char buffer[4096];
        file.read(buffer, sizeof(buffer));
        script.append(buffer, file.gcount());
    }
            
    rs::jsapi::Runtime rt;
    
    auto app = new Application(rt, "com.ripcordsoftware.examples.gtk", argc, argv);
    rs::jsapi::Global::DefineProperty(rt, "app", *app);
    
    auto builder = new Builder(rt);
    rs::jsapi::Global::DefineProperty(rt, "builder", *builder);
    
    rt.Evaluate(script.c_str());
    
    return 0;
}