#include "cad_basic_box.h"

CAD_basic_box::CAD_basic_box() : CADitem(CADitem::Basic_Box)
{
    this->description = "Basic|Box";

    pos_bot_1 = QVector3D(-1, -1,  1);
    pos_bot_2 = QVector3D( 1, -1,  1);
    pos_bot_3 = QVector3D( 1, -1, -1);
    pos_bot_4 = QVector3D(-1, -1, -1);
    pos_top_1 = QVector3D(-1,  1,  1);
    pos_top_2 = QVector3D( 1,  1,  1);
    pos_top_3 = QVector3D( 1,  1, -1);
    pos_top_4 = QVector3D(-1,  1, -1);
}

void CAD_basic_box::calculate()
{
    this->snap_basepoint = ((pos_bot_1 +
                            pos_bot_2 +
                            pos_bot_3 +
                            pos_bot_4 +
                            pos_top_1 +
                            pos_top_2 +
                            pos_top_3 +
                            pos_top_4) / 8.0);

    this->snap_vertices.append(pos_bot_1);
    this->snap_vertices.append(pos_bot_2);
    this->snap_vertices.append(pos_bot_3);
    this->snap_vertices.append(pos_bot_4);
    this->snap_vertices.append(pos_top_1);
    this->snap_vertices.append(pos_top_2);
    this->snap_vertices.append(pos_top_3);
    this->snap_vertices.append(pos_top_4);

    // Bottom edge centers
    this->snap_center.append((pos_bot_1 + pos_bot_2) / 2.0);
    this->snap_center.append((pos_bot_2 + pos_bot_3) / 2.0);
    this->snap_center.append((pos_bot_3 + pos_bot_4) / 2.0);
    this->snap_center.append((pos_bot_4 + pos_bot_1) / 2.0);
    // Top edge centers
    this->snap_center.append((pos_top_1 + pos_top_2) / 2.0);
    this->snap_center.append((pos_top_2 + pos_top_3) / 2.0);
    this->snap_center.append((pos_top_3 + pos_top_4) / 2.0);
    this->snap_center.append((pos_top_4 + pos_top_1) / 2.0);
    // Connection edge centers
    this->snap_center.append((pos_bot_1 + pos_top_1) / 2.0);
    this->snap_center.append((pos_bot_2 + pos_top_2) / 2.0);
    this->snap_center.append((pos_bot_3 + pos_top_3) / 2.0);
    this->snap_center.append((pos_bot_4 + pos_top_4) / 2.0);

    qDebug() << "calculating a box";
}
