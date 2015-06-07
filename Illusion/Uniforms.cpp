//
//  Uniforms.cpp
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#include "Uniforms.h"

template<> void Uniform<glm::vec3>::apply() {
    
}

template <typename T>
//void Uniform::Uniform(std::string name, T vec) {
//    
//}

Uniforms::Uniforms(GLuint program) {
    _shaderProgram = program;
}

void Uniforms::addUniform(UniformGeneric *uniform) {
    _uniformList.push_back(uniform);
}



void Uniforms::applyUniforms() {
}

void Uniforms::apply(glm::vec3 &vec3) {
    std::cout << "Apply vec3" << std::endl;
}