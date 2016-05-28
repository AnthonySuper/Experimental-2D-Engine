#include "script_engine.hpp"

namespace NM {
    ScriptEngine::ScriptEngine() {
        mrb = mrb_open();
    }
    
    mrb_state* ScriptEngine::getMRB() {
        return mrb;
    }
    
    mrb_value ScriptEngine::runString(std::string line) {
        return mrb_load_string(mrb, line.c_str());
    }
    
    mrb_value ScriptEngine::loadPath(std::string path) {
        FILE *fp = fopen(path.c_str(), "r");
        if(! fp) {
            throw FileNotFoundError(path);
        }
        mrb_value v = mrb_load_file(mrb, fp);
        fclose(fp);
        return v;
    }
    
}