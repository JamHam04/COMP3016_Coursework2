#version 460
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 locNormal;
layout (location = 2) in vec2 texPosition;

// Outs to fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// Uniformsq
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0)); // Get in world space

    Normal = mat3(transpose(inverse(model))) * locNormal; 

    TexCoords = texPosition; // Assign texture 

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
