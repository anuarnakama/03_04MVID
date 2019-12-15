#version 330 core

in vec3 myColor;
in vec4 myPostion; //No se como te refieres a pintarlo, si por consola o dibujarlo.
out vec4 FragColor;

uniform vec3 addColor;

void main() {
    FragColor = vec4(myColor + addColor, 1.0);
    
}