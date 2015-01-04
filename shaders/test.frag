#version 430 core

in vec4 gColor;
in vec4 gTexCoord;
in vec4 gVertexPosition;
layout (location = 40) uniform sampler2D texture0;
layout (location = 41) uniform sampler2D texture1;
layout (location = 42) uniform int UseTexture;
layout (location = 43) uniform int UseClippingX;
layout (location = 44) uniform int UseClippingY;
layout (location = 45) uniform int UseClippingZ;
layout (location = 46) uniform vec3 Depth_of_view;
layout (location = 47) uniform vec3 Height_of_intersection;


out vec4 FragColor;

void main(void)
{
    if (UseTexture == 1)
    {
        vec4 texColor0 = texture2D(texture0, vec2(gTexCoord.x, gTexCoord.y));
        vec4 texColor1 = texture2D(texture1, gTexCoord.xy);
        FragColor = texColor0 + gColor;
        return;
    }

    if( (UseClippingX == 1) && ((gVertexPosition.x > Height_of_intersection.x) || (gVertexPosition.x < Depth_of_view.x)) )
        discard;
    if( (UseClippingY == 1) && ((gVertexPosition.y > Height_of_intersection.y) || (gVertexPosition.y < Depth_of_view.y)) )
        discard;
    if( (UseClippingZ == 1) && ((gVertexPosition.z > Height_of_intersection.z) || (gVertexPosition.z < Depth_of_view.z)) )
        discard;

    FragColor = gColor;
}
