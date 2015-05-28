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

#include "dxfreaderinterface.h"

DxfReaderInterface::DxfReaderInterface()
{

}

void DxfReaderInterface::addHeader(const DRW_Header *data)
{

}

void DxfReaderInterface::addLType(const DRW_LType &data)
{

}

void DxfReaderInterface::addLayer(const DRW_Layer &data)
{

}

void DxfReaderInterface::addDimStyle(const DRW_Dimstyle &data)
{

}

void DxfReaderInterface::addVport(const DRW_Vport &data)
{

}

void DxfReaderInterface::addTextStyle(const DRW_Textstyle &data)
{

}

void DxfReaderInterface::addBlock(const DRW_Block &data)
{

}

void DxfReaderInterface::setBlock(const int handle)
{

}

void DxfReaderInterface::endBlock()
{

}

void DxfReaderInterface::addPoint(const DRW_Point &data)
{

}

void DxfReaderInterface::addLine(const DRW_Line &data)
{

}

void DxfReaderInterface::addRay(const DRW_Ray &data)
{

}

void DxfReaderInterface::addXline(const DRW_Xline &data)
{

}

void DxfReaderInterface::addArc(const DRW_Arc &data)
{

}

void DxfReaderInterface::addCircle(const DRW_Circle &data)
{

}

void DxfReaderInterface::addEllipse(const DRW_Ellipse &data)
{

}

void DxfReaderInterface::addLWPolyline(const DRW_LWPolyline &data)
{

}

void DxfReaderInterface::addPolyline(const DRW_Polyline &data)
{

}

void DxfReaderInterface::addSpline(const DRW_Spline *data)
{

}

void DxfReaderInterface::addKnot(const DRW_Entity &data)
{

}

void DxfReaderInterface::addInsert(const DRW_Insert &data)
{

}

void DxfReaderInterface::addTrace(const DRW_Trace &data)
{

}

void DxfReaderInterface::add3dFace(const DRW_3Dface &data)
{

}

void DxfReaderInterface::addSolid(const DRW_Solid &data)
{

}

void DxfReaderInterface::addMText(const DRW_MText &data)
{

}

void DxfReaderInterface::addText(const DRW_Text &data)
{

}

void DxfReaderInterface::addDimAlign(const DRW_DimAligned *data)
{

}

void DxfReaderInterface::addDimLinear(const DRW_DimLinear *data)
{

}

void DxfReaderInterface::addDimRadial(const DRW_DimRadial *data)
{

}

void DxfReaderInterface::addDimDiametric(const DRW_DimDiametric *data)
{

}

void DxfReaderInterface::addDimAngular(const DRW_DimAngular *data)
{

}

void DxfReaderInterface::addDimAngular3P(const DRW_DimAngular3p *data)
{

}

void DxfReaderInterface::addDimOrdinate(const DRW_DimOrdinate *data)
{

}

void DxfReaderInterface::addLeader(const DRW_Leader *data)
{

}

void DxfReaderInterface::addHatch(const DRW_Hatch *data)
{

}

void DxfReaderInterface::addViewport(const DRW_Viewport &data)
{

}

void DxfReaderInterface::addImage(const DRW_Image *data)
{

}

void DxfReaderInterface::linkImage(const DRW_ImageDef *data)
{

}

void DxfReaderInterface::addComment(const char *comment)
{

}

void DxfReaderInterface::writeHeader(DRW_Header &data)
{

}

void DxfReaderInterface::writeBlocks()
{

}

void DxfReaderInterface::writeBlockRecords()
{

}

void DxfReaderInterface::writeEntities()
{

}

void DxfReaderInterface::writeLTypes()
{

}

void DxfReaderInterface::writeLayers()
{

}

void DxfReaderInterface::writeTextstyles()
{

}

void DxfReaderInterface::writeVports()
{

}

void DxfReaderInterface::writeDimstyles()
{

}

