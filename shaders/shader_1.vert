#version 330

in vec3 VertexPosition;
in vec4 VertexColor;
uniform mat4x4 Matrix;

out vec4 Color;

void main(void)
{
    Color = VertexColor;
    gl_Position = Matrix * vec4(VertexPosition, 1.0);
}
