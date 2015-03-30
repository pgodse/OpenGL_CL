#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normalVector;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

out vec3 vertWorldSpace;
out vec3 transformedNormal;

uniform vec3 dirLightVec;
uniform vec3 dirLightColor;

void main(){
    vertWorldSpace = (modelMatrix * vec4(vertexPosition_modelspace, 1.0)).xyz;
    transformedNormal = (normalMatrix * normalize(vec4(normalVector, 1.0))).xyz;
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
}