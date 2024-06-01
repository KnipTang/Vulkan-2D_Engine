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














//layout(location = 0) in vec2 inPosition;
//layout(location = 1) in vec3 inColor;
//layout(location = 2) in float inTest;
////layout(location = 2) in vec2 inTexCoord;
//
//layout(location = 0) out vec3 fragColor;
////layout(location = 1) out vec2 fragTexCoord;
//
//void main() {
//    gl_Position = vec4(inPosition, 0.0, inTest);
//    fragColor = inColor;
//    //fragTexCoord = inTexCoord;
//}

//#version 450
//
//layout(binding = 0) uniform UniformBufferObject {
//    mat4 model;
//    mat4 view;
//    mat4 proj;
//} ubo;
//
//layout(location = 0) in vec2 inPosition;
//layout(location = 1) in vec3 inColor;
//layout(location = 2) in vec2 inTexCoord;
//
//layout(location = 0) out vec3 fragColor;
//layout(location = 1) out vec2 fragTexCoord;
//
//void main() {
//    gl_Position = ubo.model * vec4(inPosition, 0.0, 1.0);
//    //gl_Position = vec4(inPosition, 0.0, 1.0);
//    fragColor = inColor;
//    fragTexCoord = inTexCoord;
//}