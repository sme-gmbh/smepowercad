/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#version 430 core

in vec4 vColor;
in vec4 vTexCoord;
in vec4 vVertexPosition;
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
        vec4 texColor0 = texture2D(texture0, vec2(vTexCoord.x, vTexCoord.y));
        vec4 texColor1 = texture2D(texture1, vTexCoord.xy);
        FragColor = texColor0 + vColor;
        return;
    }

    if( (UseClippingX == 1) && ((vVertexPosition.x > Height_of_intersection.x) || (vVertexPosition.x < Depth_of_view.x)) )
        discard;
    if( (UseClippingY == 1) && ((vVertexPosition.y > Height_of_intersection.y) || (vVertexPosition.y < Depth_of_view.y)) )
        discard;
    if( (UseClippingZ == 1) && ((vVertexPosition.z > Height_of_intersection.z) || (vVertexPosition.z < Depth_of_view.z)) )
        discard;

    FragColor = vColor;
}
