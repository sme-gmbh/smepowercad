#ifndef CREATIONINTERFACE_H
#define CREATIONINTERFACE_H

#include <QColor>
#include <dxflib/src/dl_creationadapter.h>
#include "itemdb.h"

class CreationInterface : public DL_CreationAdapter
{
public:
    CreationInterface(ItemDB* itemDB);

    QColor getColorFromDXFcolorNumber(int colorNumber);

    /**
     * Called for every code / value tuple of the DXF file. The complete DXF file
     * contents can be handled by the implemetation of this function.
     */
    virtual void processCodeValuePair(unsigned int groupCode, const std::string& groupValue);

    /**
     * Called when a section (entity, table entry, etc.) is finished.
     */
    virtual void endSection();

    /**
     * Called for every layer.
     */
    virtual void addLayer(const DL_LayerData& data);

    /**
     * Called for every block. Note: all entities added after this
     * command go into this block until endBlock() is called.
     *
     * @see endBlock()
     */
    virtual void addBlock(const DL_BlockData& data);

    /** Called to end the current block */
    virtual void endBlock();

    /** Called for every text style */
    virtual void addTextStyle(const DL_StyleData& data);

    /** Called for every point */
    virtual void addPoint(const DL_PointData& data);

    /** Called for every line */
    virtual void addLine(const DL_LineData& data);

    /** Called for every arc */
    virtual void addArc(const DL_ArcData& data);

    /** Called for every circle */
    virtual void addCircle(const DL_CircleData& data);

    /** Called for every ellipse */
    virtual void addEllipse(const DL_EllipseData& data);

    /** Called for every polyline start */
    virtual void addPolyline(const DL_PolylineData& data);

    /** Called for every polyline vertex */
    virtual void addVertex(const DL_VertexData& data);

    /** Called for every spline */
    virtual void addSpline(const DL_SplineData& data);

    /** Called for every spline control point */
    virtual void addControlPoint(const DL_ControlPointData& data);

    /** Called for every spline fit point */
    virtual void addFitPoint(const DL_FitPointData& data);

    /** Called for every spline knot value */
    virtual void addKnot(const DL_KnotData& data);

    /** Called for every insert. */
    virtual void addInsert(const DL_InsertData& data);

    /** Called for every trace start */
    virtual void addTrace(const DL_TraceData& data);

    /** Called for every 3dface start */
    virtual void add3dFace(const DL_3dFaceData& data);

    /** Called for every solid start */
    virtual void addSolid(const DL_SolidData& data);


    /** Called for every Multi Text entity. */
    virtual void addMText(const DL_MTextData& data);

    /**
     * Called for additional text chunks for MTEXT entities.
     * The chunks come at 250 character in size each. Note that
     * those chunks come <b>before</b> the actual MTEXT entity.
     */
    virtual void addMTextChunk(const std::string& text);

    /** Called for every Text entity. */
    virtual void addText(const DL_TextData& data);

    /**
     * Called for every aligned dimension entity.
     */
    virtual void addDimAlign(const DL_DimensionData& data,
                             const DL_DimAlignedData& edata);
    /**
     * Called for every linear or rotated dimension entity.
     */
    virtual void addDimLinear(const DL_DimensionData& data,
                              const DL_DimLinearData& edata);

    /**
     * Called for every radial dimension entity.
     */
    virtual void addDimRadial(const DL_DimensionData& data,
                              const DL_DimRadialData& edata);

    /**
     * Called for every diametric dimension entity.
     */
    virtual void addDimDiametric(const DL_DimensionData& data,
                                 const DL_DimDiametricData& edata);

    /**
     * Called for every angular dimension (2 lines version) entity.
     */
    virtual void addDimAngular(const DL_DimensionData& data,
                               const DL_DimAngularData& edata);

    /**
     * Called for every angular dimension (3 points version) entity.
     */
    virtual void addDimAngular3P(const DL_DimensionData& data,
                                 const DL_DimAngular3PData& edata);

    /**
     * Called for every ordinate dimension entity.
     */
    virtual void addDimOrdinate(const DL_DimensionData& data,
                                const DL_DimOrdinateData& edata);

    /**
     * Called for every leader start.
     */
    virtual void addLeader(const DL_LeaderData& data);

    /**
     * Called for every leader vertex
     */
    virtual void addLeaderVertex(const DL_LeaderVertexData& data);

    /**
     * Called for every hatch entity.
     */
    virtual void addHatch(const DL_HatchData& data);

    /**
     * Called for every image entity.
     */
    virtual void addImage(const DL_ImageData& data);

    /**
     * Called for every image definition.
     */
    virtual void linkImage(const DL_ImageDefData& data);

    /**
     * Called for every hatch loop.
     */
    virtual void addHatchLoop(const DL_HatchLoopData& data);

    /**
     * Called for every hatch edge entity.
     */
    virtual void addHatchEdge(const DL_HatchEdgeData& data);

    /**
     * Called for every XRecord with the given handle.
     */
    virtual void addXRecord(const std::string& handle);

    /**
     * Called for XRecords of type string.
     */
    virtual void addXRecordString(int code, const std::string& value);

    /**
     * Called for XRecords of type double.
     */
    virtual void addXRecordReal(int code, double value);

    /**
     * Called for XRecords of type int.
     */
    virtual void addXRecordInt(int code, int value);

    /**
     * Called for XRecords of type bool.
     */
    virtual void addXRecordBool(int code, bool value);

    /**
     * Called for every beginning of an XData section of the given application.
     */
    virtual void addXDataApp(const std::string& appId);

    /**
     * Called for XData tuples.
     */
    virtual void addXDataString(int code, const std::string& value);

    /**
     * Called for XData tuples.
     */
    virtual void addXDataReal(int code, double value);

    /**
     * Called for XData tuples.
     */
    virtual void addXDataInt(int code, int value);

    /**
     * Called for dictionary objects.
     */
    virtual void addDictionary(const DL_DictionaryData& data);

    /**
     * Called for dictionary entries.
     */
    virtual void addDictionaryEntry(const DL_DictionaryEntryData& data);

    /**
     * Called after an entity has been completed.
     */
    virtual void endEntity();

    /**
     * Called for every comment in the DXF file (code 999).
     */
    virtual void addComment(const std::string& comment);

    /**
     * Called for every vector variable in the DXF file (e.g. "$EXTMIN").
     */
    virtual void setVariableVector(const std::string& key,  double v1, double v2, double v3, int code);

    /**
     * Called for every string variable in the DXF file (e.g. "$ACADVER").
     */
    virtual void setVariableString(const std::string& key, const std::string& value, int code);

    /**
     * Called for every int variable in the DXF file (e.g. "$ACADMAINTVER").
     */
    virtual void setVariableInt(const std::string& key, int value, int code);

    /**
     * Called for every double variable in the DXF file (e.g. "$DIMEXO").
     */
    virtual void setVariableDouble(const std::string& key, double value, int code);

    /**
     * Called when a SEQEND occurs (when a POLYLINE or ATTRIB is done)
     */
    virtual void endSequence();

    void printAttributes();

private:
//    QGraphicsScene* scene;
    ItemDB* itemDB;
    enum
    {
        CMD_NONE,
        CMD_POLYLINE,
        CMD_SPLINE,
        CMD_3DFACE,
        CMD_SOLID,
        CMD_LEADER

    } activeCommand;

    QString activeBlock;
    QString mTextBuffer;
    CADitem* activeItem;
};

#endif // CREATIONINTERFACE_H
