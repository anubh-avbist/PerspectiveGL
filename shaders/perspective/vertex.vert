#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
#define M_PI 3.1415926535897932384626433832795

out vec2 TexCoord;

out vec4 frag;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{


    float greenValue = sin(time) +0.5f;
    float redValue = sin(time+2*M_PI/3) +0.5f;
    float blueValue = sin(time+4*M_PI/3) +0.5f;
    
    frag = vec4(redValue, greenValue, blueValue, 1.0);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}