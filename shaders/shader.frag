#version 330 core

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shine;
};

struct Light {
    vec3 lightPosCoord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 NormalCoord;
out vec4 FragColor;

uniform vec3 u_viewPos;
uniform vec3 u_fragPos;
uniform mat3 u_normalMatrix;

uniform Material u_material;
uniform Light u_light;

void main() {
    vec3 norm = normalize(u_normalMatrix * NormalCoord);
    vec3 light_diraction = normalize(u_light.lightPosCoord - u_fragPos);
    vec3 view_diraction = normalize(u_viewPos - u_fragPos);
    vec3 reflected_diraction = reflect(-light_diraction, norm);

    FragColor = texture(TextureData1, TexCoord);
}
