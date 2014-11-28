#version 440

in vec4 Color;
in vec4 vTexCoord;
in vec4 vVertexPosition;
flat in vec3 vDepth_of_view;
flat in vec3 vHeight_of_intersection;
flat in int vUseTexture;
flat in int vUseClippingX;
flat in int vUseClippingY;
flat in int vUseClippingZ;
uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 FragColor;

void main(void)
{
    if (vUseTexture == 1)
    {
        vec4 texColor0 = texture2D(texture0, vec2(vTexCoord.x, vTexCoord.y));
        vec4 texColor1 = texture2D(texture1, vTexCoord.xy);
        FragColor = texColor0;
        return;
    }

    if( (vUseClippingX == 1) && ((vVertexPosition.x > vHeight_of_intersection.x) || (vVertexPosition.x < vDepth_of_view.x)) )
        discard;
    if( (vUseClippingY == 1) && ((vVertexPosition.y > vHeight_of_intersection.y) || (vVertexPosition.y < vDepth_of_view.y)) )
        discard;
    if( (vUseClippingZ == 1) && ((vVertexPosition.z > vHeight_of_intersection.z) || (vVertexPosition.z < vDepth_of_view.z)) )
        discard;

    FragColor = Color;
}
