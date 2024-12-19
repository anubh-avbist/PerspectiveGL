#version 330 core
#define M_PI 3.1415926535897932384626433832795
layout (location = 0) in vec3 aPos;
layout (location = 1) in float col;
layout (location = 2) in vec2 aTexCoord;

uniform float time;
uniform mat4 transformation;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 frag;
out vec2 TexCoord;

void main(){
    gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    
    float timeValue = time + col;
    float greenValue = sin(timeValue) +0.5f;
    float redValue = sin(timeValue+2*M_PI/3) +0.5f;
    float blueValue = sin(timeValue+4*M_PI/3) +0.5f;
    frag = vec4(redValue, greenValue, blueValue, 1.0);
    TexCoord = aTexCoord;

}