#version 430

layout ( lines ) in;
layout ( line_strip, max_vertices = 2 ) out;

in vec4 Color[];
in vec4 vTexCoord[];
in vec4 vVertexPosition[];

out vec4 gColor;
out vec4 gTexCoord;
out vec4 gVertexPosition;

void main()
{
//    for (int i=0; i < 2; i++)
//    {
//        vec4 pos = gl_in[i].gl_Position;
//        if (pos.x < -1.0)   return;
//        if (pos.x >  1.0)   return;
//        if (pos.y < -1.0)   return;
//        if (pos.y >  1.0)   return;
//        if (pos.z < -1.0)   return;
//        if (pos.z >  1.0)   return;
//    }

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
    EndPrimitive();
}
