#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec3 ambient;
    float shine;
};

struct Light {
    vec3 lightPosCoord;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

in vec2 TexCoord;
in vec3 NormalCoord;
in vec3 fragPos;
out vec4 FragColor;

uniform vec3 u_viewPos;
uniform Material u_material;
uniform Light u_light;

// reflect (func) returns a normlalized vec3 if the porvided vectors are
// normalized
void main() {
    float d, s;
    vec3 retAmbient, retDiffuse, retSpecular;
    vec3 norm = NormalCoord;

    vec3 light_diraction = normalize(u_light.lightPosCoord - fragPos);
    vec3 view_diraction = normalize(u_viewPos - fragPos);
    vec3 reflected_diraction = reflect(-light_diraction, norm);

    d = max(dot(light_diraction, norm), 0.0f);
    s = pow(max(dot(view_diraction, reflected_diraction), 0.0f), u_material.shine);

    retAmbient = u_light.ambient * u_material.ambient;
    retDiffuse = u_light.diffuse * (d * texture(u_material.diffuse, TexCoord).rgb);
    retSpecular = u_light.specular * (s * texture(u_material.specular, TexCoord).rgb);

    FragColor = vec4(retAmbient + retDiffuse + retSpecular, 1.0f);
}
