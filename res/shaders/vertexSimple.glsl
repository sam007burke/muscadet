#version 410 core

layout(location = 0) in vec3 position;

uniform mat4 u_modelMatrix;
uniform mat4 u_perspectiveMatrix;
uniform mat4 u_viewMatrix;

void main() {
    vec4 newPosition = u_perspectiveMatrix * u_viewMatrix * u_modelMatrix * vec4(position, 1.0f);
    gl_Position = newPosition;
}