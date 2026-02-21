#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            if (!vShaderFile.is_open() || !fShaderFile.is_open())
            {
                std::cerr << "ERROR: couldn't open shader file!" << std::endl;
                std::cerr << "check path: " << vertexPath << " and " << fragmentPath << std::endl;
                return;
            }

            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            std::cout << "vertex shader loaded " << vertexCode.length() << " chars" << std::endl;
            std::cout << "fragment shader loaded " << fragmentCode.length() << " chars" << std::endl;
        }
        catch (std::ifstream::failure &e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY" << std::endl;
        }

        const char *vShaderSource = vertexCode.c_str();
        const char *fShaderSource = fragmentCode.c_str();

        // compile shaders
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderSource, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderSource, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() { glUseProgram(ID); }

private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILCATION_ERROR of type " << type << std::endl;
                std::cout << "--- ERROR LOG ---" << infoLog << "\n"
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type " << type << std::endl;
                std::cout << "--- ERROR LOG ---" << infoLog << "\n"
                          << std::endl;
            }
        }
    }
};
#endif