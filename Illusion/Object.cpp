//
//  Object.cpp
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#include "Object.h"
#include "shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <vector>
#include "objloader.hpp"
#include "Uniforms.h"

Object::Object() {
    
}

Object::Object(std::string vertexSource, std::string fragmentSource) {
    shaderProgram = LoadShaders(vertexSource.c_str(), fragmentSource.c_str());
    
    initGeometry();
}

void Object::destroy() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteProgram(shaderProgram);
}

void Object::setProjectionViewMatrix(glm::mat4 projMat, glm::mat4 viewMat) {
    _viewMatrix = viewMat;
    _projView = projMat * viewMat;
    _normalMatrix = glm::inverse(_modelMatrix);
    _normalMatrix = glm::transpose(_normalMatrix);
}

glm::vec3 lightDir = glm::vec3(-1.0, 0.5, 0.3);
glm::vec3 lightColor = glm::vec3(0.4, 0.5, 0.6);

void Object::drawObject() {
    //Compute MVP
    glm::mat4 MVP = _projView * _modelMatrix;
    
    //Use the shader
    glUseProgram(shaderProgram);
    
    // 1st attribute buffers : vertex
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffers: normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    //Apply uniform variables
    glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(uniformModelMat, 1, GL_FALSE, &_modelMatrix[0][0]);
    glUniformMatrix4fv(uniformViewMat, 1, GL_FALSE, &_viewMatrix[0][0]);
    glUniformMatrix4fv(uniformNormalMat, 1, GL_FALSE, &_normalMatrix[0][0]);
    glUniform3f(dirColorUniform, lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(dirVecUniform, lightDir.x, lightDir.y, lightDir.z);
    
    glDrawArrays(GL_TRIANGLES, 0, _polyCount);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Object::initGeometry() {
    _projView = glm::mat4(1.0);
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(0.5, .5, .5));
    
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = loadOBJ("box.obj", vertices, uvs, normals);
    
    _polyCount = sizeof(vertices) / sizeof(GL_FLOAT);
    _polyCount = (int)vertices.size(); //For triangles
    
    //Load Vertices to VBO
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
    //Load Normals to VBO
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    
    //Get uniform locations
    uniformMVP = glGetUniformLocation(shaderProgram, "MVP");
    uniformModelMat = glGetUniformLocation(shaderProgram, "modelMatrix");
    uniformViewMat = glGetUniformLocation(shaderProgram, "viewMatrix");
    
    uniformNormalMat = glGetUniformLocation(shaderProgram, "normalMatrix");
    dirColorUniform = glGetUniformLocation(shaderProgram, "dirLightColor");
    dirVecUniform = glGetUniformLocation(shaderProgram, "dirLightVec");
    
    //Uniforms uni("dirLightColor", new Object(), shaderProgram);
    
}
