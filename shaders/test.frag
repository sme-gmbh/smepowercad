#version 420

in vec4 Color;
in vec4 vTexCoord;
in vec4 vVertexPosition;
flat in int vDepth_of_view;
flat in int vUseTexture;
flat in int vUseClipping;
uniform sampler2D texture0;
uniform sampler2D texture1;

int disc;


//flat in vec4 vHeight_of_intersection;

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

    disc = 0;

    if (vVertexPosition.z < vDepth_of_view)
        disc += 1;

    if (vUseClipping == 1)
        disc += 1;

    if (disc == 2)
        discard;

//    if( (vUseClipping == 1))
//    {
//        if( (vVertexPosition.z < vDepth_of_view.z) )
//            discard;
//    }
    else
    {
        FragColor = vec4(Color);
    }

}
