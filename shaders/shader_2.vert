#version 430
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
