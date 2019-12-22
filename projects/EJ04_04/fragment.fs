#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 myColor;

uniform sampler2D tex_1;

void main() {

    FragColor =  texture(tex_1, TexCoord);
}