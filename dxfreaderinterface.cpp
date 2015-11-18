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

#include "logging.h"

DxfReaderInterface::DxfReaderInterface(ItemDB *itemDB)
{
    this->itemDB = itemDB;
}

void DxfReaderInterface::addHeader(const DRW_Header *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addHeader";
    qCDebug(powercad) << "comments:" << data->getComments().c_str();
    qCDebug(powercad) << "vars.size:" << data->vars.size();
    QMap<std::string,DRW_Variant*> map_vars = QMap<std::string,DRW_Variant*>(data->vars);

    foreach (std::string key, map_vars.keys())
    {
        DRW_Variant* value = map_vars.value(key);

        switch (value->type)
        {
        case DRW_Variant::STRING:
            qCDebug(powercad) << "key:" << QString(key.c_str()) << "type: String" << "val:" << QString(value->content.s->c_str());
            break;
        case DRW_Variant::INTEGER:
            qCDebug(powercad) << "key:" << QString(key.c_str()) << "type: Integer" << "val:" << value->content.i;
            break;
        case DRW_Variant::DOUBLE:
            qCDebug(powercad) << "key:" << QString(key.c_str()) << "type: Double" << "val:" << value->content.d;
            break;
        case DRW_Variant::COORD:
            qCDebug(powercad) << "key:" << QString(key.c_str()) << "type: Coord" << "val:" << QVector3D(value->content.v->x, value->content.v->y, value->content.v->z);
            break;
        case DRW_Variant::INVALID:
            qCDebug(powercad) << "DRW_Variant::INVALID";
            break;
        }
    }
}

void DxfReaderInterface::addLType(const DRW_LType &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addLType";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLayer(const DRW_Layer &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addLayer";
    Layer* layer = itemDB->addLayer(QString(data.name.c_str()));

    qCDebug(powercad) << "color" << data.color;

//    layer->pen.setColor(getColorFromDXFcolorNumber(attributes.getColor()));
//    layer->brush.setColor(getColorFromDXFcolorNumber(attributes.getColor()));
//    layer->lineType = QString(attributes.getLineType().c_str());
//    layer->width = attributes.getWidth();
}

void DxfReaderInterface::addDimStyle(const DRW_Dimstyle &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimStyle";
    Q_UNUSED(data);
}

void DxfReaderInterface::addVport(const DRW_Vport &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addVport";
    Q_UNUSED(data);
}

void DxfReaderInterface::addTextStyle(const DRW_Textstyle &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addTextStyle";
    Q_UNUSED(data);
}

void DxfReaderInterface::addBlock(const DRW_Block &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addBlock";
    Q_UNUSED(data);
}

void DxfReaderInterface::setBlock(const int handle)
{
    qCDebug(powercad) << "DxfReaderInterface::setBlock";
    Q_UNUSED(handle);
}

void DxfReaderInterface::endBlock()
{
    qCDebug(powercad) << "DxfReaderInterface::endBlock";

}

void DxfReaderInterface::addPoint(const DRW_Point &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addPoint";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLine(const DRW_Line &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addLine";
    Q_UNUSED(data);
}

void DxfReaderInterface::addRay(const DRW_Ray &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addRay";
    Q_UNUSED(data);
}

void DxfReaderInterface::addXline(const DRW_Xline &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addXline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addArc(const DRW_Arc &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addArc";
    Q_UNUSED(data);
}

void DxfReaderInterface::addCircle(const DRW_Circle &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addCircle";
    Q_UNUSED(data);
}

void DxfReaderInterface::addEllipse(const DRW_Ellipse &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addEllipse";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLWPolyline(const DRW_LWPolyline &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addLWPolyline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addPolyline(const DRW_Polyline &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addPolyline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addSpline(const DRW_Spline *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addSpline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addKnot(const DRW_Entity &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addKnot";
    Q_UNUSED(data);
}

void DxfReaderInterface::addInsert(const DRW_Insert &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addInsert";
    Q_UNUSED(data);
}

void DxfReaderInterface::addTrace(const DRW_Trace &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addTrace";
    Q_UNUSED(data);
}

void DxfReaderInterface::add3dFace(const DRW_3Dface &data)
{
    qCDebug(powercad) << "DxfReaderInterface::add3dFace";
    Q_UNUSED(data);
}

void DxfReaderInterface::addSolid(const DRW_Solid &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addSolid";
    Q_UNUSED(data);
}

void DxfReaderInterface::addMText(const DRW_MText &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addMText";
    Q_UNUSED(data);
}

void DxfReaderInterface::addText(const DRW_Text &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addText";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimAlign(const DRW_DimAligned *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimAlign";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimLinear(const DRW_DimLinear *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimLinear";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimRadial(const DRW_DimRadial *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimRadial";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimDiametric(const DRW_DimDiametric *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimDiametric";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimAngular(const DRW_DimAngular *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimAngular";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimAngular3P(const DRW_DimAngular3p *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimAngular3P";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimOrdinate(const DRW_DimOrdinate *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addDimOrdinate";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLeader(const DRW_Leader *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addLeader";
    Q_UNUSED(data);
}

void DxfReaderInterface::addHatch(const DRW_Hatch *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addHatch";
    Q_UNUSED(data);
}

void DxfReaderInterface::addViewport(const DRW_Viewport &data)
{
    qCDebug(powercad) << "DxfReaderInterface::addViewport";
    Q_UNUSED(data);
}

void DxfReaderInterface::addImage(const DRW_Image *data)
{
    qCDebug(powercad) << "DxfReaderInterface::addImage";
    Q_UNUSED(data);
}

void DxfReaderInterface::linkImage(const DRW_ImageDef *data)
{
    qCDebug(powercad) << "DxfReaderInterface::linkImage";
    Q_UNUSED(data);
}

void DxfReaderInterface::addComment(const char *comment)
{
    qCDebug(powercad) << "DxfReaderInterface::addComment";
    Q_UNUSED(comment);
}

void DxfReaderInterface::writeHeader(DRW_Header &data)
{
    qCDebug(powercad) << "DxfReaderInterface::writeHeader";
    Q_UNUSED(data);
}

void DxfReaderInterface::writeBlocks()
{
    qCDebug(powercad) << "DxfReaderInterface::writeBlocks";

}

void DxfReaderInterface::writeBlockRecords()
{
    qCDebug(powercad) << "DxfReaderInterface::writeBlockRecords";

}

void DxfReaderInterface::writeEntities()
{
    qCDebug(powercad) << "DxfReaderInterface::writeEntities";

}

void DxfReaderInterface::writeLTypes()
{
    qCDebug(powercad) << "DxfReaderInterface::writeLTypes";

}

void DxfReaderInterface::writeLayers()
{
    qCDebug(powercad) << "DxfReaderInterface::writeLayers";

}

void DxfReaderInterface::writeTextstyles()
{
    qCDebug(powercad) << "DxfReaderInterface::writeTextstyles";

}

void DxfReaderInterface::writeVports()
{
    qCDebug(powercad) << "DxfReaderInterface::writeVports";

}

void DxfReaderInterface::writeDimstyles()
{
    qCDebug(powercad) << "DxfReaderInterface::writeDimstyles";

}

