#version 410 core

in vec3 v_colour;

out vec4 colour;

void main() {
    colour = vec4(v_colour, 1.0);
}