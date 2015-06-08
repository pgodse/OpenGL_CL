varying vec4 color;
varying vec2 vUVCoord;
varying vec3 viewNormal;

varying vec3 vWorldEyeVector;

void main(void)
{
    vUVCoord = gl_MultiTexCoord0.xy;
    color = vec4(gl_Normal, 1.0);
    viewNormal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}