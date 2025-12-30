#version 460
out vec4 FragColor;
uniform vec4 colourIn;

//in vec2 texCoordFrag;
// uniform sampler2D textureIn;

// Texture
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

// Lighting
in vec3 FragPos; // Fragment position in world space
in vec3 Normal; // Normal vector in world space

uniform vec3 lightPos; // Light position
uniform vec3 lightColor; // Light color
uniform vec3 viewPos; // Camera position

uniform bool isTextured;
uniform bool lightEnabled;
uniform vec4 modelColor;


void main()
{

    vec3 norm = normalize(Normal); // Normalize
    vec3 lightDir = normalize(lightPos - FragPos); // Light direction



    // Ambient (global light)
    vec3 ambient = 0.4 * lightColor; // Brightness + color


    // Diffuse 
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = diff * lightColor; // Apply diffuse

    // Specular 
    vec3 viewDir = normalize(viewPos - FragPos); // Calculate view direction
    vec3 halfwayDir = normalize(lightDir + viewDir); // Halfway between light and view
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0); // Calculate specular (16 shininess
    vec3 specular = spec * lightColor * 0.9; // Apply specular (0.2 intensity)

    vec3 lighting = ambient + diffuse + specular; // Combine lighting



    // Use texture, default to orange if no texture
    vec4 baseColor = isTextured ? texture(texture_diffuse1, TexCoords) : modelColor; // orange

    // Use lighting if enabled
    FragColor = lightEnabled ? vec4(lighting, 1.0) * baseColor : baseColor;
    //FragColor = vec4(lighting, 1.0) * baseColor; // Apply lighting to base color

}
