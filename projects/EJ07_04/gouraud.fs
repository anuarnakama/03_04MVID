#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;


void main() {

    FragColor = vec4(fragPos, 1.0f);
}