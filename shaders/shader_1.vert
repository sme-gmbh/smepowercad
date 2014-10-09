#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec4 TexCoord;
uniform int UseTexture;
uniform mat4x4 Matrix;

out vec4 Color;
out vec4 vTexCoord;
flat out int vUseTexture;

void main(void)
{
    Color = VertexColor;
    vTexCoord = TexCoord;
    vUseTexture = UseTexture;
    gl_Position = Matrix * vec4(VertexPosition, 1.0);
}
