#version 450

layout(binding = 0) uniform GlobalUBO {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 uv0;


void main() {
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
    uv0 = inTexCoord;
}