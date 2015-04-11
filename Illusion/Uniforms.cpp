//
//  Uniforms.cpp
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#include "Uniforms.h"

Uniforms::Uniforms(GLuint program) {
    _shaderProgram = program;
}

void Uniforms::addUniform(std::string name, glm::vec3 value) {
    GLint uniformLocation = glGetUniformLocation(_shaderProgram, name.c_str());
    if (uniformLocation == -1) {
        std::cout << "Warning:: Couldn't find uniform in shader program " << name << std::endl;
        return;
    }
    
    Uniform uni;
    uni.valueVec3 = value;
    uni.uniformLocation = uniformLocation;
    uni.type = Type_Uniform3f;
    
    _uniformList.push_back(uni);
}



void Uniforms::applyUniforms() {
    std::vector<Uniform>::iterator itr = _uniformList.begin();
    while (itr != _uniformList.end()) {
        if (itr->type == Type_Uniform3f && itr->uniformLocation != -1) {
            glUniform3f(itr->uniformLocation, itr->valueVec3.x, itr->valueVec3.x, itr->valueVec3.z);
        }
        itr++;
    }
    
}