#include "cad_basic_sphere.h"
#include "glwidget.h"
#include <GL/glu.h>

CAD_basic_sphere::CAD_basic_sphere() : CADitem(CADitem::Basic_Sphere)
{
    this->description = "Basic|Sphere";
    this->radius = 0.0;

    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Radius", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_basic_sphere::~CAD_basic_sphere()
{

}

QList<CADitem::ItemType> CAD_basic_sphere::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_sphere::wizardImage()
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

QString CAD_basic_sphere::iconPath()
{
    return ":/icons/cad_basic/cad_basic_sphere.svg";
}

QString CAD_basic_sphere::domain()
{
    return "Basic";
}

void CAD_basic_sphere::calculate()
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

    // tbd.: Bounding box
}

void CAD_basic_sphere::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    radius = wizardParams.value("Radius").toDouble();
}

void CAD_basic_sphere::paint(GLWidget *glwidget)
{
    QColor color_pen = getColorPen();
    QColor color_brush = getColorBrush();

    GLdouble radius = (GLdouble)radius;

    GLUquadricObj* sphere = gluNewQuadric();

    glwidget->glPushMatrix();
    glwidget->glTranslatef(position.x(), position.y(), position.z());
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricTexture(sphere, GL_TRUE);

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush);
        gluSphere(sphere, radius, 32, 32);
    }
    if (glwidget->render_outline)
    {
        glwidget->glLineWidth(1.0);
        glwidget->setPaintingColor(color_pen);
        glwidget->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        gluSphere(sphere, radius * 1.001, 32, 32);
        glwidget->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glwidget->glPopMatrix();
    gluDeleteQuadric(sphere);
}
