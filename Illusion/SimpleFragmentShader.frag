#version 330 core
out vec3 color;
in vec3 vertWorldSpace;
in vec3 transformedNormal;

uniform vec3 dirLightVec;
uniform vec3 dirLightColor;

void main()
{

    float lightVal = max(dot(dirLightVec, transformedNormal), 0);
    
    color = vec3(0.3, 0.5, 0.2) + dirLightColor * lightVal;
}