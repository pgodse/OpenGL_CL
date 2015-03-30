//
//  Object.h
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#ifndef __Illusion__Object__
#define __Illusion__Object__

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

class Object {
    GLuint shaderProgram;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    int _polyCount;
    //Uniforms
    GLuint uniformMVP, uniformModelMat, uniformViewMat, uniformNormalMat;
    GLuint dirVecUniform, dirColorUniform;
    glm::mat4 _projView, _viewMatrix, _normalMatrix, _modelMatrix;
    void destroy();
public:
    Object();
    Object(std::string vertexSource, std::string fragmentSource);
    
    void setProjectionViewMatrix(glm::mat4 projMat, glm::mat4 viewMat);
    virtual void drawObject();
    void initGeometry();
    
};
#endif /* defined(__Illusion__Object__) */
