#version 460
out vec4 FragColor;
uniform vec4 colourIn;

in vec2 texCoordFrag;
uniform sampler2D textureIn;

uniform bool isTextured;

void main()
{
    //FragColor = texture(textureIn, texCoordFrag);
    if (isTextured)
        FragColor = texture(textureIn, texCoordFrag);
    else
        FragColor = vec4(1.0, 0.25, 0.0, 1.0); 
    
}