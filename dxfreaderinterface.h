/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** Parts of this file are taken from the libdxfrw library header files
** and have been written by the libdxfrw team.
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

#ifndef DXFREADERINTERFACE_H
#define DXFREADERINTERFACE_H

#include <libdxfrw0/libdxfrw.h>
#include <libdxfrw0/drw_interface.h>


class DxfReaderInterface : public DRW_Interface
{
public:
    DxfReaderInterface();


    /** Called when header is parsed.  */
    virtual void addHeader(const DRW_Header* data);

    /** Called for every line Type.  */
    virtual void addLType(const DRW_LType& data);
    /** Called for every layer. */
    virtual void addLayer(const DRW_Layer& data);
    /** Called for every dim style. */
    virtual void addDimStyle(const DRW_Dimstyle& data);
    /** Called for every VPORT table. */
    virtual void addVport(const DRW_Vport& data);
    /** Called for every text style. */
    virtual void addTextStyle(const DRW_Textstyle& data);

    /**
     * Called for every block. Note: all entities added after this
     * command go into this block until endBlock() is called.
     *
     * @see endBlock()
     */
    virtual void addBlock(const DRW_Block& data);

    /**
     * In DWG called when the following entities corresponding to a
     * block different from the current. Note: all entities added after this
     * command go into this block until setBlock() is called already.
     *
     * int handle are the value of DRW_Block::handleBlock added with addBlock()
     */
    virtual void setBlock(const int handle);

    /** Called to end the current block */
    virtual void endBlock();

    /** Called for every point */
    virtual void addPoint(const DRW_Point& data);

    /** Called for every line */
    virtual void addLine(const DRW_Line& data);

    /** Called for every ray */
    virtual void addRay(const DRW_Ray& data);

    /** Called for every xline */
    virtual void addXline(const DRW_Xline& data);

    /** Called for every arc */
    virtual void addArc(const DRW_Arc& data);

    /** Called for every circle */
    virtual void addCircle(const DRW_Circle& data);

    /** Called for every ellipse */
    virtual void addEllipse(const DRW_Ellipse& data);

    /** Called for every lwpolyline */
    virtual void addLWPolyline(const DRW_LWPolyline& data);

    /** Called for every polyline start */
    virtual void addPolyline(const DRW_Polyline& data);

    /** Called for every spline */
    virtual void addSpline(const DRW_Spline* data);

    /** Called for every spline knot value */
    virtual void addKnot(const DRW_Entity& data);

    /** Called for every insert. */
    virtual void addInsert(const DRW_Insert& data);

    /** Called for every trace start */
    virtual void addTrace(const DRW_Trace& data);

    /** Called for every 3dface start */
    virtual void add3dFace(const DRW_3Dface& data);

    /** Called for every solid start */
    virtual void addSolid(const DRW_Solid& data);


    /** Called for every Multi Text entity. */
    virtual void addMText(const DRW_MText& data);

    /** Called for every Text entity. */
    virtual void addText(const DRW_Text& data);

    /**
     * Called for every aligned dimension entity.
     */
    virtual void addDimAlign(const DRW_DimAligned *data);
    /**
     * Called for every linear or rotated dimension entity.
     */
    virtual void addDimLinear(const DRW_DimLinear *data);

    /**
     * Called for every radial dimension entity.
     */
    virtual void addDimRadial(const DRW_DimRadial *data);

    /**
     * Called for every diametric dimension entity.
     */
    virtual void addDimDiametric(const DRW_DimDiametric *data);

    /**
     * Called for every angular dimension (2 lines version) entity.
     */
    virtual void addDimAngular(const DRW_DimAngular *data);

    /**
     * Called for every angular dimension (3 points version) entity.
     */
    virtual void addDimAngular3P(const DRW_DimAngular3p *data);

    /**
     * Called for every ordinate dimension entity.
     */
    virtual void addDimOrdinate(const DRW_DimOrdinate *data);

    /**
     * Called for every leader start.
     */
    virtual void addLeader(const DRW_Leader *data);

    /**
     * Called for every hatch entity.
     */
    virtual void addHatch(const DRW_Hatch *data);

    /**
     * Called for every viewport entity.
     */
    virtual void addViewport(const DRW_Viewport& data);

    /**
     * Called for every image entity.
     */
    virtual void addImage(const DRW_Image *data);

    /**
     * Called for every image definition.
     */
    virtual void linkImage(const DRW_ImageDef *data);

    /**
     * Called for every comment in the DXF file (code 999).
     */
    virtual void addComment(const char* comment);

    /** Sets the current attributes for entities. */
/*    void setExtrusion(double dx, double dy, double dz, double elevation) {
        extrusion->setDirection(dx, dy, dz);
        extrusion->setElevation(elevation);
    }*/

    /** @return the current attributes used for new entities. */
//    DL_Extrusion* getExtrusion() {
//        return extrusion;
//    }

    virtual void writeHeader(DRW_Header& data);
    virtual void writeBlocks();
    virtual void writeBlockRecords();
    virtual void writeEntities();
    virtual void writeLTypes();
    virtual void writeLayers();
    virtual void writeTextstyles();
    virtual void writeVports();
    virtual void writeDimstyles();
};

#endif // DXFREADERINTERFACE_H
