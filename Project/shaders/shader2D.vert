#version 450

layout(push_constant) uniform PushConstants {
    vec3 color;  // Assuming we have 3 different colors for the vertices
} pc;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragColor = pc.color;  // Use different colors based on the vertex index
}
