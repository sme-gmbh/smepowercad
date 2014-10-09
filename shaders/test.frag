#version 420

in vec4 Color;
in vec4 vTexCoord;
flat in int vUseTexture;

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
    }
    else
    {
        FragColor = vec4(Color);
    }
}
