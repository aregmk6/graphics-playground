#version 330 core

layout(location = 0) in vec3 inVerCoord;
layout(location = 1) in vec3 inNormalCoord;
layout(location = 2) in vec2 inTexCoord;

out vec2 TexCoord;

uniform mat4 u_pvm;

void main() {
    gl_Position = u_pvm * vec4(inVerCoord, 1.0f);
    TexCoord = inTexCoord;
}
