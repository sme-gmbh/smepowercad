#version 440

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec4 TexCoord;
uniform int UseTexture;
uniform int UseClipping;
uniform mat4x4 Matrix;
uniform int Depth_of_view;
//uniform vec3 Height_of_intersection;

out vec4 Color;
out vec4 vTexCoord;
flat out int vDepth_of_view;
flat out int vUseTexture;
flat out int vUseClipping;
//flat out vec4 vHeight_of_intersection;
out vec4 vVertexPosition;

void main(void)
{
    Color = VertexColor;
    vTexCoord = TexCoord;
    vUseTexture = UseTexture;
    vUseClipping = UseClipping;
    vDepth_of_view = Depth_of_view;
//    vHeight_of_intersection = Matrix * vec4(Height_of_intersection, 1.0);
    vVertexPosition = vec4(VertexPosition, 1.0);
    gl_Position = Matrix * vec4(VertexPosition, 1.0);
}
