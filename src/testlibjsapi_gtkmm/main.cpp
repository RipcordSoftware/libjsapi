#include <gtkmm.h>

#include <string>
#include <fstream>
#include <iostream>

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
        }
        
        script += ";\n";
    }
    
    return std::move(script);
}

int main(int argc, char** argv) {  
 
    try {
        std::string script = LoadScript(argc, argv);

        rs::jsapi::Runtime rt(1024 * 1024 * 1024, true, true);

        auto app = new Application(rt, "com.ripcordsoftware.examples.gtk", 1, argv);
        rs::jsapi::Global::DefineProperty(rt, "app", *app);

        auto builder = new Builder(rt);
        rs::jsapi::Global::DefineProperty(rt, "builder", *builder);
        
        rs::jsapi::Global::DefineFunction(rt, "trace", 
            [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result){
                for (auto arg : args) {
                    std::cout << arg.ToString();
                }
                
                if (args.size() > 0) {
                    std::cout << std::endl;
                }
                
                return true;
            });

        rt.Evaluate(script.c_str());
            
        //rs::jsapi::Script scr(rt, script.c_str());
        //scr.Compile();
        //scr.Execute();
    } catch (const rs::jsapi::ScriptException& ex) {
        std::cerr << 
            "ERROR: line " << ex.lineno << std::endl <<
            ex.what() << std::endl <<
            ex.linebuf << std::endl;        
    }
    
    return 0;
}