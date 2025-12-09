#version 460
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texPosition;
uniform mat4 mvpIn;
out vec2 texCoordFrag;

void main() {

    gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
    texCoordFrag = texPosition;
}