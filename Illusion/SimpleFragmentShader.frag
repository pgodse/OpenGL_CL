#version 330 core
out vec3 color;
in vec3 vertWorldSpace;
in vec3 transformedNormal;

uniform vec3 dirLightVec;
uniform vec3 dirLightColor;

uniform vec3 baseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;

void main()
{

    float lightVal = max(dot(dirLightVec, transformedNormal), 0);
    
    color = baseColor * dirLightColor * lightVal + ambientColor;
}