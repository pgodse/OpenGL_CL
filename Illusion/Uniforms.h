//
//  Uniforms.h
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#ifndef __Illusion__Uniforms__
#define __Illusion__Uniforms__

#include <OpenGL/gl3.h>
#include <iostream>
#include <vector>
#include <map>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

class UniformGeneric {
    virtual void apply();
};


template <class T>
class Uniform : public UniformGeneric {
    T value;
    std::string _name;
public:
    Uniform(std::string name, T vec3) {
        value = vec3;
        _name = name;
    }
    
    virtual void apply() = 0;
};

class Uniforms {
    enum UniformType{
        Type_Matrix4fv = (1 << 0),
        Type_Uniform3f = (1 << 1)
    };
    
    std::vector<UniformGeneric*> _uniformList;
    
    GLuint _shaderProgram;
    
public:
    Uniforms(GLuint program);
    void addUniform(UniformGeneric *uniform);
    
    void applyUniforms();
    
    void apply(glm::vec3 &vec3);
};

#endif /* defined(__Illusion__Uniforms__) */
