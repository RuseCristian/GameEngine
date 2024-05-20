#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform bool applyTint; // Whether to apply the tint

void main()
{
    vec4 color = texture(screenTexture, texCoord);
    
    // Apply purple tint only if applyTint is true
    if (applyTint)
    {
        color.rgb += vec3(0.5, 0.0, 0.5); // Adjust the values as needed
    }
    
    FragColor = color;
}
