#include "render/gl_program.hpp"

namespace NM {
    
    enum programType {
        shader,
        program
    };
    
    void checkShaderCompilation(GLuint id, programType t) {
        GLint suc;
        GLchar info[2048];
        if(t == shader) {
            glGetShaderiv(id, GL_COMPILE_STATUS, &suc);
        }
        else {
            glGetProgramiv(id, GL_LINK_STATUS, &suc);
        }
        if(suc) {
            return;
        }
        if(t == shader) {
            glGetShaderInfoLog(id, 2048, NULL, info);
        }
        else {
            glGetProgramInfoLog(id, 2048, NULL, info);
        }
        const char *i = static_cast<const char *>(info);
        std::string str = i;
        throw new GLProgram::CompileError(str);
    }
    
    GLProgram::GLProgram(const GLProgram &g) {
        id = g.id;
    }
    
    void GLProgram::set(std::string id, GLfloat val) {
        glUniform1f(getLoc(id), val);
    }
    
    void GLProgram::set(std::string name, GLint value) {
        glUniform1i(getLoc(name), value);
    }
    
    void GLProgram::set(std::string name, glm::mat4 m) {
        glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(m));
    }
    
    void GLProgram::set(std::string name, glm::vec4 v) {
        glUniform4f(getLoc(name), v.x, v.y, v.z, v.w);
    }
    
    GLint GLProgram::getLoc(std::string name) {
        return glGetUniformLocation(id, name.c_str());
    }
    
    
    void GLProgram::use(){
        glUseProgram(id);
    }
    
    GLProgram GLProgram::fromFiles(std::string vertPath, std::string fragPath) {
        std::string v = Util::readFileString(vertPath);
        std::string f = Util::readFileString(fragPath);
        return GLProgram(v, f);
    }
    
    GLProgram::GLProgram(GLProgram::SourceType ver,
            GLProgram::SourceType frag) {
        GLuint vertId, fragId;
        vertId = glCreateShader(GL_VERTEX_SHADER);
        fragId = glCreateShader(GL_FRAGMENT_SHADER);
        const char *v_str = ver.c_str();
        const char *f_str = frag.c_str();
        glShaderSource(vertId, 1, &v_str, nullptr);
        glShaderSource(fragId, 1, &f_str, nullptr);
        glCompileShader(vertId);
        checkShaderCompilation(vertId, shader);
        glCompileShader(fragId);
        checkShaderCompilation(fragId, shader);
        id = glCreateProgram();
        glAttachShader(id, vertId);
        glAttachShader(id, fragId);
        glLinkProgram(id);
        checkShaderCompilation(id, program);
        glDeleteShader(fragId);
        glDeleteShader(vertId);
    }
}
