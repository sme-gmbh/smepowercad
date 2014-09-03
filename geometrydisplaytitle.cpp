#include "geometrydisplaytitle.h"
#include "ui_geometrydisplaytitle.h"

GeometryDisplayTitle::GeometryDisplayTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeometryDisplayTitle)
{
    ui->setupUi(this);
}

GeometryDisplayTitle::~GeometryDisplayTitle()
{
    delete ui;
}
