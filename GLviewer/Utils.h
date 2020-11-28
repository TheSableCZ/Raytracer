//
// Z příkladů, které byly tvořeny na přednáškách od Tomáše Mileta.
//

#ifndef RAYTRACER_UTILS_H
#define RAYTRACER_UTILS_H

#include<GL/glew.h>
#include<iostream>

GLuint createShader(GLenum type,std::string const&src){
    auto id = glCreateShader(type);

    char const*srcs[] = {
            src.data()
    };

    glShaderSource(id,1,srcs,nullptr);

    glCompileShader(id);

    GLint status;
    glGetShaderiv(id,GL_COMPILE_STATUS,&status);

    if(status != GL_TRUE){
        GLint errLen = 0;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&errLen);
        auto buf = std::string(" ",errLen);


        glGetShaderInfoLog(id,buf.size(),&errLen,(char*)buf.data());


        throw std::runtime_error("Compilation failed: "+buf);
    }

    return id;
}

template<typename...ARGS>
GLuint createProgram(ARGS const&...ids){
    auto id = glCreateProgram();

    auto dum = {(glAttachShader(id,ids),0)...};
    (void)dum;

    auto dum2 = {(glDeleteShader(ids),0)...};
    (void)dum2;


    glLinkProgram(id);


    GLint status;
    glGetProgramiv(id,GL_LINK_STATUS,&status);

    if(status != GL_TRUE){
        GLint errLen = 0;
        glGetProgramiv(id,GL_INFO_LOG_LENGTH,&errLen);
        auto buf = std::string(" ",errLen);


        glGetProgramInfoLog(id,buf.size(),&errLen,(char*)buf.data());


        throw std::runtime_error("Linking failed: "+buf);
    }


    return id;
}

#endif //RAYTRACER_UTILS_H
