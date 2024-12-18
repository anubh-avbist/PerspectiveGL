#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader {
    public:
    Shader() {
        
    }

    static GLuint make_module(const std::string& filepath, GLenum module_type){
        std::ifstream file;
        std::stringstream bufferedLines;
        std::string line;
        file.open(filepath);
        while (std::getline(file, line)){
            bufferedLines << line << "\n";
        }
        bufferedLines << "\0";

        std::string src = bufferedLines.str();
        const char* shaderSource = src.c_str();
        bufferedLines.str("");
        file.close();


        GLuint shaderModule = glCreateShader(module_type);
        glShaderSource(shaderModule, 1, &shaderSource, NULL);
        glCompileShader(shaderModule);

        int success;
        char infoLog[512];
        glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

        if(!success){
            glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);
            std::cout << "Shader compilation error!\n" << infoLog;
        } 
        return shaderModule;
    }


    template <size_t N>
    static GLuint make_shader(GLenum (&modules)[N]){
        GLuint shaderProgram = glCreateProgram();
        for(GLuint module : modules){
            glAttachShader(shaderProgram, module);
        }
        glLinkProgram(shaderProgram);

        int success;
        char infoLog[512];
        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);

        if(!success){
            glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog;
        } 
        glLinkProgram(shaderProgram);
        return shaderProgram;
    }
};