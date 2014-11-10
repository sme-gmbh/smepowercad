#include "cad_air_duct.h"

CAD_air_duct::CAD_air_duct() : CADitem(CADitem::Air_Duct)
{
    this->description = "Air|Duct";
    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));

    wizardParams.insert(QObject::tr("Length (l)"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Width (b)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Height (a)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Flange size"), QVariant::fromValue(1.0));

    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));

    main_duct = new CAD_basic_duct;
    flange_duct_left = new CAD_basic_duct;
    flange_duct_right = new CAD_basic_duct;

    subItems.append(main_duct);
    subItems.append(flange_duct_left);
    subItems.append(flange_duct_right);
}

QList<CADitem::ItemType> CAD_air_duct::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
    flangable_items.append(CADitem::Air_DuctTransition);
    flangable_items.append(CADitem::Air_DuctTransitionRectRound);
    flangable_items.append(CADitem::Air_DuctTurn);
    flangable_items.append(CADitem::Air_DuctYpiece);
    return flangable_items;
}

QImage CAD_air_duct::wizardImage()
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

void CAD_air_duct::calculate()
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

    this->snap_flanges.append((pos_top_3 + pos_bot_2) / 2.0);
    this->snap_flanges.append((pos_top_4 + pos_bot_1) / 2.0);


    main_duct->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(position.x()));
    main_duct->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(position.y()));
    main_duct->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(position.z()));
    main_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    main_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    main_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    main_duct->wizardParams.insert("Length", QVariant::fromValue(length));
    main_duct->wizardParams.insert("Width", QVariant::fromValue(width));
    main_duct->wizardParams.insert("Height", QVariant::fromValue(height));
    main_duct->wizardParams.insert("Wall thickness", QVariant::fromValue(wall_thickness));

    flange_duct_left->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(position.x()));
    flange_duct_left->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(position.y()));
    flange_duct_left->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(position.z()));
    flange_duct_left->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_duct_left->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_duct_left->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_duct_left->wizardParams.insert("Length", QVariant::fromValue(flange_size));
    flange_duct_left->wizardParams.insert("Width", QVariant::fromValue(width + 2 * flange_size));
    flange_duct_left->wizardParams.insert("Height", QVariant::fromValue(height + 2* flange_size));
    flange_duct_left->wizardParams.insert("Wall thickness", QVariant::fromValue(wall_thickness));

    flange_duct_right->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(position.x() + length - flange_size));
    flange_duct_right->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(position.y()));
    flange_duct_right->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(position.z()));
    flange_duct_right->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_duct_right->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_duct_right->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_duct_right->wizardParams.insert("Length", QVariant::fromValue(flange_size));
    flange_duct_right->wizardParams.insert("Width", QVariant::fromValue(width + 2 * flange_size));
    flange_duct_right->wizardParams.insert("Height", QVariant::fromValue(height + 2* flange_size));
    flange_duct_right->wizardParams.insert("Wall thickness", QVariant::fromValue(wall_thickness));


//        flange_duct_left->angle_x = item->angle_x;
//        flange_duct_left->angle_y = item->angle_y;
//        flange_duct_left->angle_z = item->angle_z;
//        flange_duct_left->position = item->position;

//        flange_duct_left->pos_bot_1.setX(item->pos_bot_1.x());
//        flange_duct_left->pos_bot_1.setY(item->pos_bot_1.y() - item->flange_size);
//        flange_duct_left->pos_bot_1.setZ(item->pos_bot_1.z() - item->flange_size);

//        flange_duct_left->pos_bot_2.setX(item->pos_bot_1.x() + item->flange_size);
//        flange_duct_left->pos_bot_2.setY(item->pos_bot_1.y() - item->flange_size);
//        flange_duct_left->pos_bot_2.setZ(item->pos_bot_1.z() - item->flange_size);

//        flange_duct_left->pos_bot_3.setX(item->pos_bot_4.x() + item->flange_size);
//        flange_duct_left->pos_bot_3.setY(item->pos_bot_4.y() + item->flange_size);
//        flange_duct_left->pos_bot_3.setZ(item->pos_bot_4.z() - item->flange_size);

//        flange_duct_left->pos_bot_4.setX(item->pos_bot_4.x());
//        flange_duct_left->pos_bot_4.setY(item->pos_bot_4.y() + item->flange_size);
//        flange_duct_left->pos_bot_4.setZ(item->pos_bot_4.z() - item->flange_size);



//        flange_duct_left->pos_top_1.setX(item->pos_top_1.x());
//        flange_duct_left->pos_top_1.setY(item->pos_top_1.y() - item->flange_size);
//        flange_duct_left->pos_top_1.setZ(item->pos_top_1.z() + item->flange_size);

//        flange_duct_left->pos_top_2.setX(item->pos_top_1.x() + item->flange_size);
//        flange_duct_left->pos_top_2.setY(item->pos_top_1.y() - item->flange_size);
//        flange_duct_left->pos_top_2.setZ(item->pos_top_1.z() + item->flange_size);

//        flange_duct_left->pos_top_3.setX(item->pos_top_4.x() + item->flange_size);
//        flange_duct_left->pos_top_3.setY(item->pos_top_4.y() + item->flange_size);
//        flange_duct_left->pos_top_3.setZ(item->pos_top_4.z() + item->flange_size);

//        flange_duct_left->pos_top_4.setX(item->pos_top_4.x());
//        flange_duct_left->pos_top_4.setY(item->pos_top_4.y() + item->flange_size);
//        flange_duct_left->pos_top_4.setZ(item->pos_top_4.z() + item->flange_size);

//        flange_duct_left->wall_thickness = item->flange_size;

//        paintBasicDuct(layer, flange_duct_left);

//        flange_duct_right->angle_x = item->angle_x;
//        flange_duct_right->angle_y = item->angle_y;
//        flange_duct_right->angle_z = item->angle_z;
//        flange_duct_right->position = item->position;

//        flange_duct_right->pos_bot_1.setX(item->pos_bot_2.x() - item->flange_size);
//        flange_duct_right->pos_bot_1.setY(item->pos_bot_2.y() - item->flange_size);
//        flange_duct_right->pos_bot_1.setZ(item->pos_bot_2.z() - item->flange_size);

//        flange_duct_right->pos_bot_2.setX(item->pos_bot_2.x());
//        flange_duct_right->pos_bot_2.setY(item->pos_bot_2.y() - item->flange_size);
//        flange_duct_right->pos_bot_2.setZ(item->pos_bot_2.z() - item->flange_size);

//        flange_duct_right->pos_bot_3.setX(item->pos_bot_3.x());
//        flange_duct_right->pos_bot_3.setY(item->pos_bot_3.y() + item->flange_size);
//        flange_duct_right->pos_bot_3.setZ(item->pos_bot_3.z() - item->flange_size);

//        flange_duct_right->pos_bot_4.setX(item->pos_bot_3.x() - item->flange_size);
//        flange_duct_right->pos_bot_4.setY(item->pos_bot_3.y() + item->flange_size);
//        flange_duct_right->pos_bot_4.setZ(item->pos_bot_3.z() - item->flange_size);



//        flange_duct_right->pos_top_1.setX(item->pos_top_2.x() - item->flange_size);
//        flange_duct_right->pos_top_1.setY(item->pos_top_2.y() - item->flange_size);
//        flange_duct_right->pos_top_1.setZ(item->pos_top_2.z() + item->flange_size);

//        flange_duct_right->pos_top_2.setX(item->pos_top_2.x());
//        flange_duct_right->pos_top_2.setY(item->pos_top_2.y() - item->flange_size);
//        flange_duct_right->pos_top_2.setZ(item->pos_top_2.z() + item->flange_size);

//        flange_duct_right->pos_top_3.setX(item->pos_top_3.x());
//        flange_duct_right->pos_top_3.setY(item->pos_top_3.y() + item->flange_size);
//        flange_duct_right->pos_top_3.setZ(item->pos_top_3.z() + item->flange_size);

//        flange_duct_right->pos_top_4.setX(item->pos_top_3.x() - item->flange_size);
//        flange_duct_right->pos_top_4.setY(item->pos_top_3.y() + item->flange_size);
//        flange_duct_right->pos_top_4.setZ(item->pos_top_3.z() + item->flange_size);

//        flange_duct_right->wall_thickness = item->flange_size;
//       //
//        paintBasicDuct(layer, flange_duct_right);


}

void CAD_air_duct::processWizardInput()
{
    QVector3D size;

    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());

    size.setX(wizardParams.value(QObject::tr("Length (l)")).toDouble());
    size.setY(wizardParams.value(QObject::tr("Width (b)")).toDouble());
    size.setZ(wizardParams.value(QObject::tr("Height (a)")).toDouble());
    length = wizardParams.value(QObject::tr("Length (l)")).toDouble();
    width = wizardParams.value(QObject::tr("Width (b)")).toDouble();

    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    wall_thickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();
    flange_size = wizardParams.value(QObject::tr("Flange size")).toDouble();




//    pos_bot_1 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position;
//    pos_bot_2 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position;
//    pos_bot_3 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position;
//    pos_bot_4 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position;

//    pos_top_1 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position;
//    pos_top_2 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position;
//    pos_top_3 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position;
//    pos_top_4 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position;

    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;

    pos_top_1 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
    pos_top_2 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
    pos_top_3 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
    pos_top_4 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;




}
