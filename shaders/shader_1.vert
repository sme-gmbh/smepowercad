#version 440

layout (location = 0) in vec3 VertexPosition;
layout (location = 11) in vec4 VertexColor;
layout (location = 12) in vec4 TexCoord;
uniform int UseTexture;
uniform int UseClippingX;
uniform int UseClippingY;
uniform int UseClippingZ;
uniform mat4x4 Matrix;
uniform vec3 Depth_of_view;
uniform vec3 Height_of_intersection;

out vec4 Color;
out vec4 vTexCoord;
flat out vec3 vDepth_of_view;
flat out vec3 vHeight_of_intersection;
flat out int vUseTexture;
flat out int vUseClippingX;
flat out int vUseClippingY;
flat out int vUseClippingZ;
out vec4 vVertexPosition;

void main(void)
{
    Color = VertexColor;
    vTexCoord = TexCoord;
    vUseTexture = UseTexture;
    vUseClippingX = UseClippingX;
    vUseClippingY = UseClippingY;
    vUseClippingZ = UseClippingZ;
    vDepth_of_view = Depth_of_view;
    vHeight_of_intersection = Height_of_intersection;
    vVertexPosition = vec4(VertexPosition, 1.0);
    gl_Position = Matrix * vec4(VertexPosition, 1.0);
}
