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

CAD_air_duct::~CAD_air_duct()
{
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

//    matrix_rotation.setToIdentity();
//    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
//    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
//    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

//    pos_bot_1 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
//    pos_bot_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
//    pos_bot_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
//    pos_bot_4 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);

//    pos_top_1 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
//    pos_top_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
//    pos_top_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
//    pos_top_4 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);





    main_duct->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(position.x()));
    main_duct->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(position.y()));
    main_duct->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(position.z()));
    main_duct->wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(angle_x));
    main_duct->wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(angle_y));
    main_duct->wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(angle_z));
    main_duct->wizardParams.insert(QObject::tr("Length (l)"), QVariant::fromValue(length));
    main_duct->wizardParams.insert(QObject::tr("Width (b)"), QVariant::fromValue(width));
    main_duct->wizardParams.insert(QObject::tr("Height (a)"), QVariant::fromValue(height));
    main_duct->wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(wall_thickness));
    main_duct->processWizardInput();
    main_duct->calculate();


    QVector3D pos_rot = position + (QVector3D(-(length - flange_size), 0.0, 0.0));
    flange_duct_left->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(pos_rot.x()));
    flange_duct_left->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(pos_rot.y()));
    flange_duct_left->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(pos_rot.z()));
    flange_duct_left->wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(angle_x));
    flange_duct_left->wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(angle_y));
    flange_duct_left->wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(angle_z));
    flange_duct_left->wizardParams.insert(QObject::tr("Length (l)"), QVariant::fromValue(flange_size));
    flange_duct_left->wizardParams.insert(QObject::tr("Width (b)"), QVariant::fromValue(width + 2 * flange_size));
    flange_duct_left->wizardParams.insert(QObject::tr("Height (a)"), QVariant::fromValue(width + 2 * flange_size));
    flange_duct_left->wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(flange_size));
    flange_duct_left->processWizardInput();
    flange_duct_left->calculate();

    flange_duct_right->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(position.x()));
    flange_duct_right->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(position.y()));
    flange_duct_right->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(position.z()));
    flange_duct_right->wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(angle_x));
    flange_duct_right->wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(angle_y));
    flange_duct_right->wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(angle_z));
    flange_duct_right->wizardParams.insert(QObject::tr("Length (l)"), QVariant::fromValue(flange_size));
    flange_duct_right->wizardParams.insert(QObject::tr("Width (b)"), QVariant::fromValue(width + 2 * flange_size));
    flange_duct_right->wizardParams.insert(QObject::tr("Height (a)"), QVariant::fromValue(height + 2 * flange_size));
    flange_duct_right->wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(flange_size));
    flange_duct_right->processWizardInput();
    flange_duct_right->calculate();
    qDebug() << "angle x" << angle_x;

    boundingBox.enterVertex(flange_duct_left->pos_top_1);
    boundingBox.enterVertex(flange_duct_left->pos_top_2);
    boundingBox.enterVertex(flange_duct_left->pos_top_3);
    boundingBox.enterVertex(flange_duct_left->pos_top_4);
    boundingBox.enterVertex(flange_duct_left->pos_bot_1);
    boundingBox.enterVertex(flange_duct_left->pos_bot_2);
    boundingBox.enterVertex(flange_duct_left->pos_bot_3);
    boundingBox.enterVertex(flange_duct_left->pos_bot_4);

    boundingBox.enterVertex(flange_duct_right->pos_top_1);
    boundingBox.enterVertex(flange_duct_right->pos_top_2);
    boundingBox.enterVertex(flange_duct_right->pos_top_3);
    boundingBox.enterVertex(flange_duct_right->pos_top_4);
    boundingBox.enterVertex(flange_duct_right->pos_bot_1);
    boundingBox.enterVertex(flange_duct_right->pos_bot_2);
    boundingBox.enterVertex(flange_duct_right->pos_bot_3);
    boundingBox.enterVertex(flange_duct_right->pos_bot_4);


    this->snap_basepoint = (position);

    this->snap_vertices.append(main_duct->snap_vertices);

    this->snap_flanges.append(main_duct->snap_flanges);





}

void CAD_air_duct::processWizardInput()
{

    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());

    size.setX(wizardParams.value(QObject::tr("Length (l)")).toDouble());
    size.setY(wizardParams.value(QObject::tr("Width (b)")).toDouble());
    size.setZ(wizardParams.value(QObject::tr("Height (a)")).toDouble());
    length = wizardParams.value(QObject::tr("Length (l)")).toDouble();
    width = wizardParams.value(QObject::tr("Width (b)")).toDouble();
    height = wizardParams.value(QObject::tr("Height (a)")).toDouble();

    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    wall_thickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();
    flange_size = wizardParams.value(QObject::tr("Flange size")).toDouble();


//    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
//    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
//    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
//    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;

//    pos_top_1 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
//    pos_top_2 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
//    pos_top_3 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
//    pos_top_4 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;

}
