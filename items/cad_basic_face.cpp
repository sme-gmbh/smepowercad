#include "cad_basic_face.h"
#include <QDebug>
#include <QPen>

CAD_basic_face::CAD_basic_face() : CADitem(CADitem::Basic_Face)
{
    this->description = "Basic|Face";

    widthByLayer = false;
    widthByBlock = false;

    this->wizardParams.insert(QObject::tr("Position x1"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position y1"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position z1"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position x2"), QVariant::fromValue(1.0));
    this->wizardParams.insert(QObject::tr("Position y2"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position z2"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Width"), QVariant::fromValue(1.0));
}

QList<CADitem::ItemType> CAD_basic_face::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_face::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_basic_face::calculate()
{
}

void CAD_basic_face::processWizardInput()
{

}
