#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D Material;

void main() {
    vec4 result = texture(Material, TexCoords);

    if(result.a < 0.1){
        discard;
    }

    FragColor = result;
}
