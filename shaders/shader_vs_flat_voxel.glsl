#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 TexCoords;

void main() {
    /*gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    FragPos = vec3(model * vec4(aPos, 0.0, 1.0));
    TexCoords = aTexCoords;*/

    vec4 worldPos = model * vec4(aPos, 0.1, 1.0);
    
    gl_Position = projection * view * worldPos;
    FragPos = vec3(worldPos);
    TexCoords = aTexCoords;
}
