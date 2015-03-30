//
//  Uniforms.cpp
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#include "Uniforms.h"

Uniforms::Uniforms(std::string name, void *value, UniformType type, GLuint program) {
    _shaderProgram = program;
    addUniform(name, value, type);
}

void Uniforms::addUniform(std::string name, void *value, UniformType type) {
    GLint uniformLocation = glGetUniformLocation(_shaderProgram, name.c_str());
    if (uniformLocation == -1) {
        std::cout << "Warning:: Couldn't find uniform in shader program " << name << std::endl;
        return;
    }
    
    Uniform uni;
    uni.value = value;
    uni.uniformLocation = uniformLocation;
    uni.type = type;
    
    _uniformList.push_back(uni);
}



void Uniforms::applyUniforms() {
    
}