#version 430 core

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3 ) out;

in vec4 Color[];
in vec4 vTexCoord[];
in vec4 vVertexPosition[];

out vec4 gColor;
out vec4 gTexCoord;
out vec4 gVertexPosition;

void main()
{
    gVertexPosition = vVertexPosition[0];
    gColor = Color[0];
    gTexCoord = vTexCoord[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gVertexPosition = vVertexPosition[1];
    gColor = Color[1];
    gTexCoord = vTexCoord[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gVertexPosition = vVertexPosition[2];
    gColor = Color[2];
    gTexCoord = vTexCoord[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
