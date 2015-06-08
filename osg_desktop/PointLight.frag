varying vec4 color;
varying vec2 vUVCoord;
varying vec3 viewNormal;

uniform vec4 ambientColor;
uniform sampler2D textureMap;

//Lights values
uniform vec3 pointLightPos;
uniform vec3 pointLightCol;
void main(void)
{
    gl_FragColor = color + texture2D(textureMap, vUVCoord);
}