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

#include "creationinterface.h"

#include <iostream>
#include <stdio.h>
#include "logging.h"

CreationInterface::CreationInterface(ItemDB *itemDB)
{
    this->itemDB = itemDB;
    activeCommand = CMD_NONE;
    activeBlock.clear();
    mTextBuffer.clear();
    activeItem = NULL;
}

QColor CreationInterface::getColorFromDXFcolorNumber(int colorNumber)
{
    QColor color;
    color.setRedF(dxfColors[colorNumber][0]);
    color.setGreenF(dxfColors[colorNumber][1]);
    color.setBlueF(dxfColors[colorNumber][2]);
    return color;
}

/**
 * Called for every code / value tuple of the DXF file. The complete DXF file
 * contents can be handled by the implemetation of this function.
 */
void CreationInterface::processCodeValuePair(unsigned int groupCode, const std::string& groupValue)
{
    Q_UNUSED(groupCode);
    Q_UNUSED(groupValue);
}

/**
 * Called when a section (entity, table entry, etc.) is finished.
 */
void CreationInterface::endSection()
{
    //printf("END_SECTION\n");
}

/**
 * Method which handles layers.
 */
void CreationInterface::addLayer(const DL_LayerData& data) {
    //printf("LAYER: %s flags: %d", data.name.c_str(), data.flags);
    printAttributes();
    // readout complete

    Layer* layer = itemDB->addLayer(QString(data.name.c_str()));
    layer->pen.setColor(getColorFromDXFcolorNumber(attributes.getColor()));
    layer->brush.setColor(getColorFromDXFcolorNumber(attributes.getColor()));
    layer->lineType = (Layer::LineType)layer->metaEnum_lineType.keysToValue(attributes.getLineType().data());
//    layer->lineType = QString(attributes.getLineType().c_str());
    layer->width = attributes.getWidth();

    // TODO: handle flags and all attributes
}

/**
 * Called for every block. Note: all entities added after this
 * command go into this block until endBlock() is called.
 *
 * @see endBlock()
 */
void CreationInterface::addBlock(const DL_BlockData& data)
{
    //printf("BLOCK: %s (%6.3f, %6.3f, %6.3f) flags: %d", data.name.c_str(), data.bpx, data.bpy, data.bpz, data.flags);
    printAttributes();
    // readout complete

    activeBlock = QString(data.name.c_str());
}

/** Called to end the current block */
void CreationInterface::endBlock()
{
    //printf("END_BLOCK");

    activeBlock = QString();
}

/** Called for every text style */
void CreationInterface::addTextStyle(const DL_StyleData& data)
{
    //printf("TEXTSTYLE: %s flags: %d", data.name.c_str(), data.flags);
    printAttributes();
//    data.bigFontFile;
//    data.bold;
//    data.fixedTextHeight;
//    data.flags;
//    data.italic;
//    data.lastHeightUsed;
//    data.name;
//    data.obliqueAngle;
//    data.primaryFontFile;
//    data.textGenerationFlags;
//    data.widthFactor;
}

/**
 * Method which handles point entities.
 */
void CreationInterface::addPoint(const DL_PointData& data) {
    //printf("POINT    (%6.3f, %6.3f, %6.3f)",
    //       data.x, data.y, data.z);
    printAttributes();
    // readout complete
}

/**
 * Method which handles line entities.
 */
void CreationInterface::addLine(const DL_LineData& data) {
    //printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)",
    //       data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
    printAttributes();
    // readout complete

    CAD_basic_line* current_cadline = new CAD_basic_line();
    current_cadline->p1 = QVector3D(data.x1, data.y1, data.z1);
    current_cadline->p2 = QVector3D(data.x2, data.y2, data.z2);
    current_cadline->calculate();

    if (attributes.getColor()==256)     // BYLAYER
    {
        current_cadline->color_pen = Qt::transparent;
    }
    else if (attributes.getColor()==0)  // BYBLOCK
    {
        //printf("BYBLOCK");
    }
    else
    {
        current_cadline->color_pen = getColorFromDXFcolorNumber(attributes.getColor());
    }

    if (attributes.getWidth() == -1)    // BYLAYER
    {
        current_cadline->widthByLayer = true;
    }
    else if (attributes.getWidth() >= 0)
    {
        current_cadline->width = attributes.getWidth() / 100.0; // to get mm
    }

    if (activeBlock.isEmpty())
        itemDB->addItem(current_cadline, attributes.getLayer().c_str());
    else
    {

    }
}

/**
 * Method which handles arc entities.
 */
void CreationInterface::addArc(const DL_ArcData& data) {
    //printf("ARC      c(%6.3f, %6.3f, %6.3f) radius: %6.3f, angle1: %6.3f, angle2: %6.3f",
    //       data.cx, data.cy, data.cz,
    //       data.radius, data.angle1, data.angle2);
    printAttributes();
    // readout complete
}

/**
 * Method which handles circle entities.
 */
void CreationInterface::addCircle(const DL_CircleData& data) {
    //printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f",
    //       data.cx, data.cy, data.cz,
    //       data.radius);
    printAttributes();
    // readout complete
}

/** Called for every ellipse */
void CreationInterface::addEllipse(const DL_EllipseData& data)
{
    //printf("ELLIPSE   c(%6.3f, %6.3f, %6.3f) m(%6.3f, %6.3f, %6.3f) angle1 %6.3f angle2 %6.3f ratio %6.3f",
    //        data.cx, data.cy, data.cz, data.mx, data.my, data.mz, data.angle1, data.angle2, data.ratio);
    printAttributes();
    // readout complete
}


/**
 * Method which handles polyline entities.
 */
void CreationInterface::addPolyline(const DL_PolylineData& data) {

    //printf("POLYLINE m: %d n: %d number: %i ",
    //       data.m, data.n, data.number);
    //printf("flags: %d", (int)data.flags);
//    printAttributes();
    // readout complete

//    data.flags;
//    data.m;
//    data.n;
//    data.number;

    CADitem* item;

    if (!data.flags & DL_PFACE_MESH)
    {
        activeCommand = CMD_POLYLINE;
        item = new CAD_basic_polyline();
//        printf("PLine3D");
        // meshes [mxn] not supported yet
    }
    else
    {
        activeCommand = CMD_3DFACE;
        item = new CAD_basic_3Dface();
//        printf("PFaceMesh");
    }

//    printf("\n");


    if (attributes.getColor()==256)     // BYLAYER
    {
        item->color_pen = Qt::transparent;
        item->color_brush = Qt::transparent;
    }
    else if (attributes.getColor()==0)  // BYBLOCK
    {
        //printf("BYBLOCK");
    }
    else
    {
        item->color_pen = getColorFromDXFcolorNumber(attributes.getColor());
        item->color_brush = getColorFromDXFcolorNumber(attributes.getColor());
    }

    if (activeBlock.isEmpty())
        itemDB->addItem(item, attributes.getLayer().c_str());
    else
    {

    }

    activeItem = item;
}


/**
 * Method which handles vertices.
 */
void CreationInterface::addVertex(const DL_VertexData& data) {
//    printf("VERTEX   (%6.3f, %6.3f, %6.3f) bulge: %6.3f\n",
//           data.x, data.y, data.z,
//           data.bulge);
//    printAttributes();
    // readout complete

    switch (activeCommand)
    {
    case CMD_NONE:
        break;
    case CMD_POLYLINE:
    {
        if (activeItem == NULL)
        {
            //printf("CreationInterface::addVertex: activeItem == NULL\n");
            return;
        }
        CAD_basic_polyline* item = (CAD_basic_polyline*) activeItem;
        CAD_basic_polyline::Vertex vertex;
        vertex.pos = QVector3D(data.x, data.y, data.z);
        vertex.widthStart = attributes.getWidth();
        vertex.widthEnd = attributes.getWidth();        // TBD. Endwidth...
        item->vertices.append(vertex);
        break;
    }
    case CMD_SPLINE:
    {
        break;
    }
    case CMD_3DFACE:
    {
        if (activeItem == NULL)
        {
            //printf("CreationInterface::addVertex: activeItem == NULL\n");
            return;
        }
        CAD_basic_3Dface* item = (CAD_basic_3Dface*) activeItem;
        CAD_basic_3Dface::Vertex vertex;
        vertex.pos = QVector3D(data.x, data.y, data.z);
        vertex.widthStart = attributes.getWidth();
        vertex.widthEnd = attributes.getWidth();        // TBD. Endwidth...
        item->vertices.append(vertex);
        break;
    }
    case CMD_SOLID:
    {
        break;
    }
    case CMD_LEADER:
    {
        break;
    }
    }
}

/** Called for every spline */
void CreationInterface::addSpline(const DL_SplineData& data)
{
    activeCommand = CMD_SPLINE;
    //printf("SPLINE   degree: %d flags: %d, nControl: %d nFit: %d, nKnots: %d, start(%6.3f, %6.3f, %6.3f), end(%6.3f, %6.3f, %6.3f)\n",
    //data.degree, data.flags, data.nControl, data.nFit, data.nKnots, data.tangentStartX, data.tangentStartY, data.tangentStartZ, data.tangentEndX, data.tangentEndY, data.tangentEndZ);
    printAttributes();
    // readout complete
}

/** Called for every spline control point */
void CreationInterface::addControlPoint(const DL_ControlPointData& data)
{
    //printf("CONTROLPT (%6.3f, %6.3f, %6.3f) w: %6.3f", data.x, data.y, data.z, data.w);
    printAttributes();
    // readout complete
}

/** Called for every spline fit point */
void CreationInterface::addFitPoint(const DL_FitPointData& data)
{
    //printf("FITPT     (%6.3f, %6.3f, %6.3f)", data.x, data.y, data.z);
    printAttributes();
    // readout complete
}

/** Called for every spline knot value */
void CreationInterface::addKnot(const DL_KnotData& data)
{
    //printf("KNOT      k: %6.3f", data.k);
    printAttributes();
    // readout complete
}

/** Called for every insert. */
void CreationInterface::addInsert(const DL_InsertData& data)
{
    //printf("INSERT    tbd.");
    //qCDebug(powercad) << data.angle << data.cols << data.colSp << data.ipx << data.ipy << data.ipz << QString(data.name.c_str()) << data.rows << data.rowSp << data.sx << data.sy << data.sz;
    printAttributes();
    // readout complete

}

/** Called for every trace start */
void CreationInterface::addTrace(const DL_TraceData& data)
{
    //printf("TRACE    thickness: %6.3f ", data.thickness);
    for (int i=0; i<4; i++) {
        //printf("   corner %d: %6.3f %6.3f %6.3f",
        //    i, data.x[i], data.y[i], data.z[i]);
    }

    printAttributes();
    // readout complete
}

void CreationInterface::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE");
    for (int i=0; i<4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f",
            i, data.x[i], data.y[i], data.z[i]);
    }
    //printf(" thickness: %6.3f", data.thickness);
    printAttributes();
    // readout complete

    activeCommand = CMD_3DFACE;

    CAD_basic_3Dface* item = new CAD_basic_3Dface();
    // meshes [mxn] not supported yet

    if (attributes.getColor()==256)     // BYLAYER
    {
        item->color_pen = Qt::transparent;
    }
    else if (attributes.getColor()==0)  // BYBLOCK
    {
        //printf("BYBLOCK");
    }
    else
    {
        attributes.getColor();  //tbd. convert color number to rgba
    }

    if (activeBlock.isEmpty())
        itemDB->addItem(item, attributes.getLayer().c_str());
    else
    {

    }

    activeItem = item;

}

/** Called for every solid start */
void CreationInterface::addSolid(const DL_SolidData& data)
{
    //printf("SOLID ");
    for (int i=0; i<4; i++) {
        //printf("   corner %d: %6.3f %6.3f %6.3f",
        //    i, data.x[i], data.y[i], data.z[i]);
    }
    //printf(" thickness: %6.3f", data.thickness);
    printAttributes();
    // readout complete

    activeCommand = CMD_SOLID;
}

/** Called for every Multi Text entity. */
void CreationInterface::addMText(const DL_MTextData& data)
{
    //printf("MULTITEXT tbd. ");
    mTextBuffer.append(data.text.c_str());
    //printf("text: %s\n", mTextBuffer.toUtf8().data());
    printAttributes();
    // many data entries!
    mTextBuffer.clear();
}

/**
 * Called for additional text chunks for MTEXT entities.
 * The chunks come at 250 character in size each. Note that
 * those chunks come <b>before</b> the actual MTEXT entity.
 */
void CreationInterface::addMTextChunk(const std::string& text)
{
    //printf("MTEXTCHUNK %s\n", text.c_str());
    mTextBuffer.append(text.c_str());
    // readout complete
}

/** Called for every Text entity. */
void CreationInterface::addText(const DL_TextData& data)
{
    //printf("MULTITEXT tbd. %s\n", data.text.c_str());
    // many data entries!
}

/**
 * Called for every aligned dimension entity.
 */
void CreationInterface::addDimAlign(const DL_DimensionData& data,
                         const DL_DimAlignedData& edata)
{

}

/**
 * Called for every linear or rotated dimension entity.
 */
void CreationInterface::addDimLinear(const DL_DimensionData& data,
                          const DL_DimLinearData& edata)
{

}

/**
 * Called for every radial dimension entity.
 */
void CreationInterface::addDimRadial(const DL_DimensionData& data,
                          const DL_DimRadialData& edata)
{

}

/**
 * Called for every diametric dimension entity.
 */
void CreationInterface::addDimDiametric(const DL_DimensionData& data,
                          const DL_DimDiametricData& edata)
{

}

/**
 * Called for every angular dimension (2 lines version) entity.
 */
void CreationInterface::addDimAngular(const DL_DimensionData& data,
                          const DL_DimAngularData& edata)
{

}

/**
 * Called for every angular dimension (3 points version) entity.
 */
void CreationInterface::addDimAngular3P(const DL_DimensionData& data,
                          const DL_DimAngular3PData& edata)
{

}

/**
 * Called for every ordinate dimension entity.
 */
void CreationInterface::addDimOrdinate(const DL_DimensionData& data,
                         const DL_DimOrdinateData& edata)
{

}

/**
 * Called for every leader start.
 */
void CreationInterface::addLeader(const DL_LeaderData& data)
{
    activeCommand = CMD_LEADER;
}

/**
 * Called for every leader vertex
 */
void CreationInterface::addLeaderVertex(const DL_LeaderVertexData& data)
{
    if (activeCommand != CMD_LEADER)
        qCDebug(powercad) << "addLeaderVertex without addLeader!";
    //printf("LEADERVERTEX (%6.3f, %6.3f, %6.3f\n)", data.x, data.y, data.z);
    // readout complete
}

/**
 * Called for every hatch entity.
 */
void CreationInterface::addHatch(const DL_HatchData& data)
{
    //printf("HATCH      angle: %6.3f, numLoops: %i, origin(%6.3f, %6.3f), pattern: %s, scale: %6.3f, ",
    //       data.angle, data.numLoops, data.originX, data.originY, data.pattern.c_str(), data.scale);
//    if (data.solid)
        //printf("solid\n");
//    else
        //printf("non-solid\n");
    // readout complete

//    data.angle;
//    data.numLoops;
//    data.originX;
//    data.originY;
//    data.pattern;
//    data.scale;
//    data.solid;
}

/**
 * Called for every image entity.
 */
void CreationInterface::addImage(const DL_ImageData& data)
{
    //printf("IMAGE      tbd.\n");
    // many data entries!
}

/**
 * Called for every image definition.
 */
void CreationInterface::linkImage(const DL_ImageDefData& data)
{
    //printf("LINKIMAGE file: %s, ref: %s\n", data.file.c_str(), data.ref.c_str());
    // readout complete
}

/**
 * Called for every hatch loop.
 */
void CreationInterface::addHatchLoop(const DL_HatchLoopData& data)
{

}

/**
 * Called for every hatch edge entity.
 */
void CreationInterface::addHatchEdge(const DL_HatchEdgeData& data)
{

}

/**
 * Called for every XRecord with the given handle.
 */
void CreationInterface::addXRecord(const std::string& handle)
{

}

/**
 * Called for XRecords of type string.
 */
void CreationInterface::addXRecordString(int code, const std::string& value)
{

}

/**
 * Called for XRecords of type double.
 */
void CreationInterface::addXRecordReal(int code, double value)
{

}

/**
 * Called for XRecords of type int.
 */
void CreationInterface::addXRecordInt(int code, int value)
{

}

/**
 * Called for XRecords of type bool.
 */
void CreationInterface::addXRecordBool(int code, bool value)
{

}

/**
 * Called for every beginning of an XData section of the given application.
 */
void CreationInterface::addXDataApp(const std::string& appId)
{

}

/**
 * Called for XData tuples.
 */
void CreationInterface::addXDataString(int code, const std::string& value)
{

}

/**
 * Called for XData tuples.
 */
void CreationInterface::addXDataReal(int code, double value)
{

}

/**
 * Called for XData tuples.
 */
void CreationInterface::addXDataInt(int code, int value)
{

}

/**
 * Called for dictionary objects.
 */
void CreationInterface::addDictionary(const DL_DictionaryData& data)
{

}

/**
 * Called for dictionary entries.
 */
void CreationInterface::addDictionaryEntry(const DL_DictionaryEntryData& data)
{

}

/**
 * Called after an entity has been completed.
 */
void CreationInterface::endEntity()
{
    //printf("END_ENTITY\n");
    switch (activeCommand)
    {
    case CMD_NONE:
        break;
    case CMD_POLYLINE:
    {
        if (activeItem == NULL)
        {
            //printf("CreationInterface::addVertex: activeItem == NULL\n");
            return;
        }
        CAD_basic_polyline* item = (CAD_basic_polyline*) activeItem;
        item->calculate();
        break;
    }
    case CMD_SPLINE:
    {
        break;
    }
    case CMD_3DFACE:
    {
        if (activeItem == NULL)
        {
            //printf("CreationInterface::addVertex: activeItem == NULL\n");
            return;
        }
        CAD_basic_3Dface* item = (CAD_basic_3Dface*) activeItem;
        item->calculate();
        break;
    }
    case CMD_SOLID:
    {
        break;
    }
    case CMD_LEADER:
    {
        break;
    }
    }

    activeCommand = CMD_NONE;
    activeItem = NULL;
}

/**
 * Called for every comment in the DXF file (code 999).
 */
void CreationInterface::addComment(const std::string& comment)
{

}

/**
 * Called for every vector variable in the DXF file (e.g. "$EXTMIN").
 */
void CreationInterface::setVariableVector(const std::string& key,  double v1, double v2, double v3, int code)
{

}

/**
 * Called for every string variable in the DXF file (e.g. "$ACADVER").
 */
void CreationInterface::setVariableString(const std::string& key, const std::string& value, int code)
{

}

/**
 * Called for every int variable in the DXF file (e.g. "$ACADMAINTVER").
 */
void CreationInterface::setVariableInt(const std::string& key, int value, int code)
{

}

/**
 * Called for every double variable in the DXF file (e.g. "$DIMEXO").
 */
void CreationInterface::setVariableDouble(const std::string& key, double value, int code)
{

}

/**
 * Called when a SEQEND occurs (when a POLYLINE or ATTRIB is done)
 */
void CreationInterface::endSequence()
{

}


void CreationInterface::printAttributes() {
    //printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
    //printf(" Color: ");
    if (attributes.getColor()==256)	{
        //printf("BYLAYER");
    } else if (attributes.getColor()==0) {
        //printf("BYBLOCK");
    } else {
        //printf("%d", attributes.getColor());
    }
    //printf(" Width: ");
    if (attributes.getWidth()==-1) {
        //printf("BYLAYER");
    } else if (attributes.getWidth()==-2) {
        //printf("BYBLOCK");
    } else if (attributes.getWidth()==-3) {
        //printf("DEFAULT");
    } else {
        //printf("%d", attributes.getWidth());
    }
    //printf(" Type: %s\n", attributes.getLineType().c_str());
}
