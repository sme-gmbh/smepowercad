#include "cad_basic_box.h"

CAD_basic_box::CAD_basic_box() : CADitem(CADitem::Box)
{
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

}
