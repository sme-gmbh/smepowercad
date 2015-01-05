#include "cad_air_heatexchangerairair.h"
#include "glwidget.h"

CAD_air_heatExchangerAirAir::CAD_air_heatExchangerAirAir() : CADitem(CADitem::Air_HeatExchangerAirAir)
{
    this->description = "Air|Heat exchanger air/air";
    this->duct = new CAD_air_duct();
    this->subItems.append(duct);
    this->seperator_1 = new CAD_basic_plane();
    this->subItems.append(seperator_1);
    this->seperator_2 = new CAD_basic_plane();
    this->subItems.append(seperator_2);
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));


    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("i", QVariant::fromValue(20.0));

    processWizardInput();
    calculate();
}

CAD_air_heatExchangerAirAir::~CAD_air_heatExchangerAirAir()
{

}

QList<CADitem::ItemType> CAD_air_heatExchangerAirAir::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctFireDamper);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
    flangable_items.append(CADitem::Air_DuctTransition);
    flangable_items.append(CADitem::Air_DuctTransitionRectRound);
    flangable_items.append(CADitem::Air_DuctTurn);
    flangable_items.append(CADitem::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitem::Air_DuctYpiece);
    flangable_items.append(CADitem::Air_Filter);
    flangable_items.append(CADitem::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_heatExchangerAirAir::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_heatExchangerAirAir::iconPath()
{
    return ":/icons/cad_air/cad_air_heatexchangerairair.svg";
}

QString CAD_air_heatExchangerAirAir::domain()
{
    return "Air";
}

void CAD_air_heatExchangerAirAir::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_he = position + matrix_rotation * QVector3D(0.0, 0.0, -a/2);
    this->duct->wizardParams.insert("Position x", QVariant::fromValue(position_he.x()));
    this->duct->wizardParams.insert("Position y", QVariant::fromValue(position_he.y()));
    this->duct->wizardParams.insert("Position z", QVariant::fromValue(position_he.z()));
    this->duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    this->duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    this->duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    this->duct->wizardParams.insert("s", QVariant::fromValue(s));
    this->duct->wizardParams.insert("l", QVariant::fromValue(l));
    this->duct->wizardParams.insert("b", QVariant::fromValue(b));
    this->duct->wizardParams.insert("a", QVariant::fromValue(2 * a));
    this->duct->wizardParams.insert("ff", QVariant::fromValue(ff));
    this->duct->wizardParams.insert("fe", QVariant::fromValue(fe));

    this->duct->processWizardInput();
    this->duct->calculate();

    QVector3D position_sp1 = position + matrix_rotation * QVector3D(0.0, -b/2 +s, -a/2);
    this->seperator_1->wizardParams.insert("Position x", QVariant::fromValue(position_sp1.x()));
    this->seperator_1->wizardParams.insert("Position y", QVariant::fromValue(position_sp1.y()));
    this->seperator_1->wizardParams.insert("Position z", QVariant::fromValue(position_sp1.z()));
    this->seperator_1->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    this->seperator_1->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    this->seperator_1->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    this->seperator_1->wizardParams.insert("Length (A)", QVariant::fromValue((l - i) / 2));
    this->seperator_1->wizardParams.insert("Width (B)", QVariant::fromValue(b-2*s));

    this->seperator_1->processWizardInput();
    this->seperator_1->calculate();

    QVector3D position_sp2 = position + matrix_rotation * QVector3D((l+i)/2, -b/2 +s, -a/2);
    this->seperator_2->wizardParams.insert("Position x", QVariant::fromValue(position_sp2.x()));
    this->seperator_2->wizardParams.insert("Position y", QVariant::fromValue(position_sp2.y()));
    this->seperator_2->wizardParams.insert("Position z", QVariant::fromValue(position_sp2.z()));
    this->seperator_2->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    this->seperator_2->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    this->seperator_2->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    this->seperator_2->wizardParams.insert("Length (A)", QVariant::fromValue((l - i) / 2));
    this->seperator_2->wizardParams.insert("Width (B)", QVariant::fromValue(b-2*s));

    this->seperator_2->processWizardInput();
    this->seperator_2->calculate();

    points[0][0] = position + matrix_rotation * QVector3D((l-i)/2, -b/2 + s, -a/2);
    points[0][1] = position + matrix_rotation * QVector3D(l/2,     -b/2 + s, -3*a/2 + s);
    points[0][2] = position + matrix_rotation * QVector3D((l+i)/2, -b/2 + s, -a/2 );
    points[0][3] = position + matrix_rotation * QVector3D(l/2,     -b/2 + s,  a/2 - s);

    points[1][0] = position + matrix_rotation * QVector3D((l-i)/2, b/2 - s, -a/2);
    points[1][1] = position + matrix_rotation * QVector3D(l/2,     b/2 - s, -3*a/2 + s);
    points[1][2] = position + matrix_rotation * QVector3D((l+i)/2, b/2 - s, -a/2);
    points[1][3] = position + matrix_rotation * QVector3D(l/2,     b/2 - s, a/2 - s);


    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, -a));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, -a));

    this->boundingBox = duct->boundingBox;

}

void CAD_air_heatExchangerAirAir::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    s = wizardParams.value("s").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    i = wizardParams.value("i").toDouble();

}

void CAD_air_heatExchangerAirAir::paint(GLWidget *glwidget)
{
    QColor color_pen = getColorPen();
    QColor color_brush = getColorBrush();

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush);
        glwidget->glBegin(GL_QUADS);

        glwidget->glVertex3f((GLfloat)points[0][0].x(), (GLfloat)points[0][0].y(), (GLfloat)points[0][0].z());
        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());
        glwidget->glVertex3f((GLfloat)points[1][0].x(), (GLfloat)points[1][0].y(), (GLfloat)points[1][0].z());

        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
        glwidget->glVertex3f((GLfloat)points[0][2].x(), (GLfloat)points[0][2].y(), (GLfloat)points[0][2].z());
        glwidget->glVertex3f((GLfloat)points[1][2].x(), (GLfloat)points[1][2].y(), (GLfloat)points[1][2].z());
        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());

        glwidget->glVertex3f((GLfloat)points[0][2].x(), (GLfloat)points[0][2].y(), (GLfloat)points[0][2].z());
        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
        glwidget->glVertex3f((GLfloat)points[1][2].x(), (GLfloat)points[1][2].y(), (GLfloat)points[1][2].z());

        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
        glwidget->glVertex3f((GLfloat)points[0][0].x(), (GLfloat)points[0][0].y(), (GLfloat)points[0][0].z());
        glwidget->glVertex3f((GLfloat)points[1][0].x(), (GLfloat)points[1][0].y(), (GLfloat)points[1][0].z());
        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
        glwidget->glEnd();

    }
    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen);
        glwidget->glLineWidth(1.0);
        glwidget->glBegin(GL_LINE_LOOP);
        glwidget->glVertex3f((GLfloat)points[0][0].x(), (GLfloat)points[0][0].y(), (GLfloat)points[0][0].z());
        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
        glwidget->glVertex3f((GLfloat)points[0][2].x(), (GLfloat)points[0][2].y(), (GLfloat)points[0][2].z());
        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
        glwidget->glEnd();
        glwidget->glBegin(GL_LINE_LOOP);
        glwidget->glVertex3f((GLfloat)points[1][0].x(), (GLfloat)points[1][0].y(), (GLfloat)points[1][0].z());
        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());
        glwidget->glVertex3f((GLfloat)points[1][2].x(), (GLfloat)points[1][2].y(), (GLfloat)points[1][2].z());
        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
        glwidget->glEnd();
        glwidget->glBegin(GL_LINES);
        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());
        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
        glwidget->glEnd();
    }
}
