#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

const vec3 lightColor = vec3(1.0, 1.0, 1.0); // Warm yellow light

void main() {
    const vec3 lightDirection = normalize(vec3(0.0, -1.0, -1.0));

    // Calculate the dot product between the normal and the light direction
    float diff = max(dot(fragNormal, lightDirection), 0.2);

    // Simple diffuse lighting
    vec3 diffuse = diff * fragColor * lightColor; // Assuming white light

    // Sample the texture
    vec4 texColor = texture(texSampler, fragTexCoord);

    // Apply diffuse lighting to the texture color
    vec3 finalColor = texColor.rgb * diffuse;

    // Set the output color
    outColor = vec4(finalColor, texColor.a);

    //outColor = vec4(fragTexCoord, 0.0, 1.0);
    //outColor = texture(texSampler, fragTexCoord);
    //outColor = texture(texSampler, fragTexCoord * 2.0);
    //outColor = vec4(fragColor * texture(texSampler, fragTexCoord).rgb, 1.0);
}