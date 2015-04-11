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
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

enum UniformType{
    Type_Matrix4fv = (1 << 0),
    Type_Uniform3f = (1 << 1)
};

class Uniforms {
    GLuint _shaderProgram;
    
    struct Uniform{
        GLint uniformLocation;
        glm::vec3 valueVec3;
        UniformType type;
    };
    
    std::vector<Uniform> _uniformList;
public:
    Uniforms(GLuint program);
    void addUniform(std::string name, glm::vec3 value);
    
    virtual void applyUniforms();
};

#endif /* defined(__Illusion__Uniforms__) */
