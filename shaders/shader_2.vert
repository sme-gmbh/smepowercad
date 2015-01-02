#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec4 TexCoord;

layout (location = 0) uniform mat4x4 Matrix;

out vec4 vColor;
out vec4 vTexCoord;
out vec4 vVertexPosition;

void main(void)
{
    vColor = VertexColor;
    vTexCoord = TexCoord;
    vVertexPosition = vec4(VertexPosition, 1.0);
    gl_Position = Matrix * vec4(VertexPosition, 1.0);
}
