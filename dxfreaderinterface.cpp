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

#include <QDebug>
#include <QMap>
#include "dxfreaderinterface.h"
#include "layer.h"

DxfReaderInterface::DxfReaderInterface(ItemDB *itemDB)
{
    this->itemDB = itemDB;
}

void DxfReaderInterface::addHeader(const DRW_Header *data)
{
    qDebug() << "DxfReaderInterface::addHeader";
    qDebug() << "comments:" << data->getComments().c_str();
    qDebug() << "vars.size:" << data->vars.size();
    QMap<std::string,DRW_Variant*> map_vars = QMap<std::string,DRW_Variant*>(data->vars);

    foreach (std::string key, map_vars.keys())
    {
        DRW_Variant* value = map_vars.value(key);

        switch (value->type)
        {
        case DRW_Variant::STRING:
            qDebug() << "key:" << QString(key.c_str()) << "type: String" << "val:" << QString(value->content.s->c_str());
            break;
        case DRW_Variant::INTEGER:
            qDebug() << "key:" << QString(key.c_str()) << "type: Integer" << "val:" << value->content.i;
            break;
        case DRW_Variant::DOUBLE:
            qDebug() << "key:" << QString(key.c_str()) << "type: Double" << "val:" << value->content.d;
            break;
        case DRW_Variant::COORD:
            qDebug() << "key:" << QString(key.c_str()) << "type: Coord" << "val:" << QVector3D(value->content.v->x, value->content.v->y, value->content.v->z);
            break;
        case DRW_Variant::INVALID:
            qDebug() << "DRW_Variant::INVALID";
            break;
        }
    }
}

void DxfReaderInterface::addLType(const DRW_LType &data)
{
    qDebug() << "DxfReaderInterface::addLType";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLayer(const DRW_Layer &data)
{
    qDebug() << "DxfReaderInterface::addLayer";
    Layer* layer = itemDB->addLayer(QString(data.name.c_str()));

    qDebug() << "color" << data.color;

//    layer->pen.setColor(getColorFromDXFcolorNumber(attributes.getColor()));
//    layer->brush.setColor(getColorFromDXFcolorNumber(attributes.getColor()));
//    layer->lineType = QString(attributes.getLineType().c_str());
//    layer->width = attributes.getWidth();
}

void DxfReaderInterface::addDimStyle(const DRW_Dimstyle &data)
{
    qDebug() << "DxfReaderInterface::addDimStyle";
    Q_UNUSED(data);
}

void DxfReaderInterface::addVport(const DRW_Vport &data)
{
    qDebug() << "DxfReaderInterface::addVport";
    Q_UNUSED(data);
}

void DxfReaderInterface::addTextStyle(const DRW_Textstyle &data)
{
    qDebug() << "DxfReaderInterface::addTextStyle";
    Q_UNUSED(data);
}

void DxfReaderInterface::addBlock(const DRW_Block &data)
{
    qDebug() << "DxfReaderInterface::addBlock";
    Q_UNUSED(data);
}

void DxfReaderInterface::setBlock(const int handle)
{
    qDebug() << "DxfReaderInterface::setBlock";
    Q_UNUSED(handle);
}

void DxfReaderInterface::endBlock()
{
    qDebug() << "DxfReaderInterface::endBlock";

}

void DxfReaderInterface::addPoint(const DRW_Point &data)
{
    qDebug() << "DxfReaderInterface::addPoint";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLine(const DRW_Line &data)
{
    qDebug() << "DxfReaderInterface::addLine";
    Q_UNUSED(data);
}

void DxfReaderInterface::addRay(const DRW_Ray &data)
{
    qDebug() << "DxfReaderInterface::addRay";
    Q_UNUSED(data);
}

void DxfReaderInterface::addXline(const DRW_Xline &data)
{
    qDebug() << "DxfReaderInterface::addXline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addArc(const DRW_Arc &data)
{
    qDebug() << "DxfReaderInterface::addArc";
    Q_UNUSED(data);
}

void DxfReaderInterface::addCircle(const DRW_Circle &data)
{
    qDebug() << "DxfReaderInterface::addCircle";
    Q_UNUSED(data);
}

void DxfReaderInterface::addEllipse(const DRW_Ellipse &data)
{
    qDebug() << "DxfReaderInterface::addEllipse";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLWPolyline(const DRW_LWPolyline &data)
{
    qDebug() << "DxfReaderInterface::addLWPolyline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addPolyline(const DRW_Polyline &data)
{
    qDebug() << "DxfReaderInterface::addPolyline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addSpline(const DRW_Spline *data)
{
    qDebug() << "DxfReaderInterface::addSpline";
    Q_UNUSED(data);
}

void DxfReaderInterface::addKnot(const DRW_Entity &data)
{
    qDebug() << "DxfReaderInterface::addKnot";
    Q_UNUSED(data);
}

void DxfReaderInterface::addInsert(const DRW_Insert &data)
{
    qDebug() << "DxfReaderInterface::addInsert";
    Q_UNUSED(data);
}

void DxfReaderInterface::addTrace(const DRW_Trace &data)
{
    qDebug() << "DxfReaderInterface::addTrace";
    Q_UNUSED(data);
}

void DxfReaderInterface::add3dFace(const DRW_3Dface &data)
{
    qDebug() << "DxfReaderInterface::add3dFace";
    Q_UNUSED(data);
}

void DxfReaderInterface::addSolid(const DRW_Solid &data)
{
    qDebug() << "DxfReaderInterface::addSolid";
    Q_UNUSED(data);
}

void DxfReaderInterface::addMText(const DRW_MText &data)
{
    qDebug() << "DxfReaderInterface::addMText";
    Q_UNUSED(data);
}

void DxfReaderInterface::addText(const DRW_Text &data)
{
    qDebug() << "DxfReaderInterface::addText";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimAlign(const DRW_DimAligned *data)
{
    qDebug() << "DxfReaderInterface::addDimAlign";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimLinear(const DRW_DimLinear *data)
{
    qDebug() << "DxfReaderInterface::addDimLinear";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimRadial(const DRW_DimRadial *data)
{
    qDebug() << "DxfReaderInterface::addDimRadial";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimDiametric(const DRW_DimDiametric *data)
{
    qDebug() << "DxfReaderInterface::addDimDiametric";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimAngular(const DRW_DimAngular *data)
{
    qDebug() << "DxfReaderInterface::addDimAngular";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimAngular3P(const DRW_DimAngular3p *data)
{
    qDebug() << "DxfReaderInterface::addDimAngular3P";
    Q_UNUSED(data);
}

void DxfReaderInterface::addDimOrdinate(const DRW_DimOrdinate *data)
{
    qDebug() << "DxfReaderInterface::addDimOrdinate";
    Q_UNUSED(data);
}

void DxfReaderInterface::addLeader(const DRW_Leader *data)
{
    qDebug() << "DxfReaderInterface::addLeader";
    Q_UNUSED(data);
}

void DxfReaderInterface::addHatch(const DRW_Hatch *data)
{
    qDebug() << "DxfReaderInterface::addHatch";
    Q_UNUSED(data);
}

void DxfReaderInterface::addViewport(const DRW_Viewport &data)
{
    qDebug() << "DxfReaderInterface::addViewport";
    Q_UNUSED(data);
}

void DxfReaderInterface::addImage(const DRW_Image *data)
{
    qDebug() << "DxfReaderInterface::addImage";
    Q_UNUSED(data);
}

void DxfReaderInterface::linkImage(const DRW_ImageDef *data)
{
    qDebug() << "DxfReaderInterface::linkImage";
    Q_UNUSED(data);
}

void DxfReaderInterface::addComment(const char *comment)
{
    qDebug() << "DxfReaderInterface::addComment";
    Q_UNUSED(comment);
}

void DxfReaderInterface::writeHeader(DRW_Header &data)
{
    qDebug() << "DxfReaderInterface::writeHeader";
    Q_UNUSED(data);
}

void DxfReaderInterface::writeBlocks()
{
    qDebug() << "DxfReaderInterface::writeBlocks";

}

void DxfReaderInterface::writeBlockRecords()
{
    qDebug() << "DxfReaderInterface::writeBlockRecords";

}

void DxfReaderInterface::writeEntities()
{
    qDebug() << "DxfReaderInterface::writeEntities";

}

void DxfReaderInterface::writeLTypes()
{
    qDebug() << "DxfReaderInterface::writeLTypes";

}

void DxfReaderInterface::writeLayers()
{
    qDebug() << "DxfReaderInterface::writeLayers";

}

void DxfReaderInterface::writeTextstyles()
{
    qDebug() << "DxfReaderInterface::writeTextstyles";

}

void DxfReaderInterface::writeVports()
{
    qDebug() << "DxfReaderInterface::writeVports";

}

void DxfReaderInterface::writeDimstyles()
{
    qDebug() << "DxfReaderInterface::writeDimstyles";

}

