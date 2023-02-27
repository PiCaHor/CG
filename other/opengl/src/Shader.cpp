#include "Shader.h"
#include <iostream>
#include <fstream> 
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std; 

Shader::Shader(const char* vertexPath, const char* franmentPath)
{
    ifstream vertexFile; 
    ifstream fragmentFile; 
    stringstream vectexSStream; 
    stringstream fragmentSStream; 

    vertexFile.open(vertexPath); 
    fragmentFile.open(franmentPath); 
    vertexFile.exceptions(ifstream::failbit | ifstream::badbit); 
    fragmentFile.exceptions(ifstream::failbit | ifstream::badbit); 
    try
    {
        if (!vertexFile.is_open() || !fragmentFile.is_open())
        {
            throw logic_error("open file error!"); 
        }

        vectexSStream << vertexFile.rdbuf(); 
        fragmentSStream << fragmentFile.rdbuf(); 

        vertexString = vectexSStream.str(); 
        fragmentString = fragmentSStream.str(); 

        vertexSource = vertexString.c_str(); 
        fragmentSource = fragmentString.c_str(); 

        unsigned int vertex, fragment; 

        vertex = glCreateShader(GL_VERTEX_SHADER); 
        glShaderSource(vertex,1,&vertexSource,NULL); 
        glCompileShader(vertex); 
        checkCompileErrors(vertex,"VERTEX"); 

        fragment = glCreateShader(GL_FRAGMENT_SHADER); 
        glShaderSource(fragment,1,&fragmentSource,NULL); 
        glCompileShader(fragment);
        checkCompileErrors(fragment,"FRAGMENT"); 

        ID = glCreateProgram(); 
        glAttachShader(ID,vertex); 
        glAttachShader(ID,fragment); 
        glLinkProgram(ID); 
        checkCompileErrors(ID,"PROGRAM"); 

        glDeleteShader(vertex); 
        glDeleteShader(fragment); 
    }
    catch(const exception & ex)
    {
        printf(ex.what()); 
    }
}

void Shader::use()
{
    glUseProgram(ID); 
}

void Shader::checkCompileErrors(unsigned int ID, std::string type)
{
    int success; 
    char infoLog[512]; 
    if (type != "PROGRAM")
    {
        glGetShaderiv(ID,GL_COMPILE_STATUS,&success); 
        if(!success)
        {
            glGetShaderInfoLog(ID, 512, NULL, infoLog); 
            cout << "shader compile error: " << infoLog<<endl; 
        }

    }
    else
    {
        glGetProgramiv(ID,GL_LINK_STATUS,&success); 
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog); 
            cout << "program linking error: " << infoLog<<endl; 
        }
    }
}


void Shader::SetUniform3f(const char* paramNameString, glm::vec3 param)
{
    glUniform3f(glGetUniformLocation(ID,paramNameString), param.x, param.y, param.z); 
}

void Shader::SetUniform1f(const char* paramNameString, float param)
{
    glUniform1f(glGetUniformLocation(ID,paramNameString), param);
}

void Shader::SetUniform1i(const char* paramNameString, int slot)
{
    glUniform1i(glGetUniformLocation(ID,paramNameString), slot); 
}