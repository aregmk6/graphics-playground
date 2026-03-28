#version 330 core

layout(location = 0) in vec3 inVerCoord;
layout(location = 1) in vec2 inTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main() {
    gl_Position = perspective * view * model * vec4(inVerCoord, 1.0f);
    TexCoord = inTexCoord;
}
