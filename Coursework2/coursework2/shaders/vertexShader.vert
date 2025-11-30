#version 460
layout (location = 0) in vec3 position;
uniform mat4 mvpIn;

void main() {

    gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
}