#version 460
out vec4 FragColor;
uniform vec4 colourIn;

in vec2 texCoordFrag;
uniform sampler2D textureIn;

uniform int bgTexture;

void main()
{
    if (bgTexture == 1)
        FragColor = texture(textureIn, texCoordFrag);
    else
        FragColor = colourIn;
    
}