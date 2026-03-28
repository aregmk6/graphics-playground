#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform float mix_amount;
uniform sampler2D TextureData1;
uniform sampler2D TextureData2;

void main() {
    vec4 curColor = texture(TextureData2, TexCoord);
    if (curColor.a > 0.01) {
        FragColor = mix(texture(TextureData1, TexCoord), texture(TextureData2, TexCoord), mix_amount);
    } else {
        FragColor = texture(TextureData1, TexCoord);
    }
}
