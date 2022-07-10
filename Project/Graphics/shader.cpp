//learnt from https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <GL/glew.h>

#include "shader.hpp"

GLuint LoadShaders(const char * vertexFile,const char * fragmentFile){

    GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vShaderInfo;
    std::ifstream vShaderStream(vertexFile, std::ios::in);
    if(vShaderStream.is_open()){
        std::stringstream sstr;
        sstr << vShaderStream.rdbuf();
        vShaderInfo = sstr.str();
        vShaderStream.close();
    }

    std::string fShaderInfo;
    std::ifstream fShaderStream(fragmentFile, std::ios::in);
    if(fShaderStream.is_open()){
        std::stringstream sstr;
        sstr << fShaderStream.rdbuf();
        fShaderInfo = sstr.str();
        fShaderStream.close();
    }

    char const * VertexSourcePointer = vShaderInfo.c_str();
    glShaderSource(vertexID, 1, &VertexSourcePointer , NULL);
    glCompileShader(vertexID);
    char const * FragmentSourcePointer = fShaderInfo.c_str();
    glShaderSource(fragmentID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fragmentID);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexID);
    glAttachShader(program, fragmentID);
    glLinkProgram(program);
    
    glDetachShader(program, vertexID);
    glDetachShader(program, fragmentID);
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    return program;
}


