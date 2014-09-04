#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent, ItemDB *itemDB) :
    QGLWidget(QGLFormat(QGL::SampleBuffers|QGL::AlphaChannel), parent)
{
    this->itemDB = itemDB;
    this->mousePos = QPoint();
    rot_x = rot_y = rot_z = 0.0f;

    makeCurrent();
    this->setMouseTracking(false);

    this->pickActive = false;
    this->cursorShown = true;
    this->snapMode = SnapCenter;

    this->setMouseTracking(true);

    this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    //this->setAttribute(Qt::WA_OpaquePaintEvent);



    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glShadeModel (GL_FLAT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0};
    GLfloat light_position[] = { -5.0, 15.0, 25.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);




    tile_list = glGenLists(1);
    glNewList(tile_list, GL_COMPILE);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    }
    glEnd();
    glEndList();
}

GLWidget::~GLWidget()
{
    glDeleteLists(tile_list, 1);
}

void GLWidget::setup(QPoint translationOffset, qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, GLWidget::CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view, qreal rot_x, qreal rot_y, qreal rot_z)
{
    this->translationOffset = translationOffset;
    this->zoomFactor = zoomFactor;
    this->centerOfViewInScene = centerOfViewInScene;
    this->displayCenter = displayCenter;
    this->cuttingplane = cuttingplane;
    this->height_of_intersection = height_of_intersection;
    this->depth_of_view = depth_of_view;
    this->rot_x = rot_x;
    this->rot_y = rot_y;
    this->rot_z = rot_z;
}

void GLWidget::moveCursor(QPoint pos)
{
//    qDebug() << "Overlay: moveCursor " << pos.x() << " " << pos.y();
    this->mousePos = pos;
    this->cursorShown = true;
    this->repaint();
}

void GLWidget::hideCursor()
{
    this->cursorShown = false;
    this->repaint();
}

void GLWidget::pickStart()
{
    this->pickActive = true;
    this->pickStartPos = this->mousePos;
}

void GLWidget::pickEnd()
{
    this->pickActive = false;
    this->repaint();
}

bool GLWidget::isPickActive()
{
    return this->pickActive;
}

QRect GLWidget::selection()
{
    // Selection rect muss immer von topleft noch bottomright gehen

    QPoint topLeft;
    topLeft.setX(qMin(this->pickStartPos.x(), this->mousePos.x()));
    topLeft.setY(qMin(this->pickStartPos.y(), this->mousePos.y()));

    QPoint bottomRight;
    bottomRight.setX(qMax(this->pickStartPos.x(), this->mousePos.x()));
    bottomRight.setY(qMax(this->pickStartPos.y(), this->mousePos.y()));

    return QRect(topLeft, bottomRight - QPoint(1, 1));
    //return QRect(this->pickStartPos.x(), this->pickStartPos.y(), this->mousePos.x() - this->pickStartPos.x(), this->mousePos.y() - this->pickStartPos.y());
}

Qt::ItemSelectionMode GLWidget::selectionMode()
{
    if (this->mousePos.x() - this->pickStartPos.x() > 0)
        return Qt::ContainsItemShape;
        //return Qt::ContainsItemBoundingRect;
    else
        return Qt::IntersectsItemShape;
        //return Qt::IntersectsItemBoundingRect;
}

void GLWidget::snap_enable(bool on)
{

}

void GLWidget::set_snap_mode(SnapMode mode)
{
    this->snapMode = mode;
}

void GLWidget::set_snapPos(QPoint snapPos)
{
    this->snapPos = snapPos;
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    saveGLState();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-(double)this->width(), (double)this->width(), -(double)this->height(), (double)this->height(), this->width(), 1000000);
    glTranslatef(0.0f, 0.0f, -500000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot_z, 0.0f, 0.0f, 1.0f);
    glScaled(this->zoomFactor, this->zoomFactor, this->zoomFactor);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);



//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

//    glColor4ub(255, 255, 255, 255);

//    glCallList(tile_list);

//    glColor4ub(0, 255, 255, 255);

//    glLineWidth(2.0);
//    glBegin(GL_LINES);
//    glVertex3f(-30000.0, 0.0, 0.0);
//    glVertex3f(30000.0, 0.0, 0.0);
//    glEnd();

//    QVector3D p1 = QVector3D(-30000.0, 2.0, 0.0);
//    QVector3D p2 = QVector3D(30000.0, 2.0, 0.0);


//    for (int i = 0; i < 3; i++)
//    {
//        glBegin(GL_LINES);
//        glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
//        glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());
//        glEnd();

//        p1 += QVector3D(0.0, 2.0, 0.0);
//        p2 += QVector3D(0.0, 2.0, 0.0);
//    }

    paintContent(itemDB->layers);

    restoreGLState();
//    glFlush();

    // Overlay


    QPen pen(Qt::white);
    pen.setWidth(1);

//    mousePos.setX(500);
//    mousePos.setY(500);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(pen);
    painter.drawLine(QPoint(0, this->mousePos.y()), QPoint(this->width() - 1, this->mousePos.y()));
    painter.drawLine(QPoint(this->mousePos.x(), 0), QPoint(this->mousePos.x(), this->height() - 1));

//    painter.end();

    glBegin(GL_LINES);
    glVertex3i(0, mousePos.y(), 0);
    glVertex3i(this->width() - 1, mousePos.y(), 0);
    glVertex3i(mousePos.x(), 0, 0);
    glVertex3i(mousePos.x(), this->height() - 1, 0);
    glEnd();

    event->accept();
}

void GLWidget::saveGLState()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void GLWidget::restoreGLState()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}


void GLWidget::paintContent(QList<Layer*> layers)
{


//    glBegin(GL_TRIANGLES);
//    glVertex3f(-30000.0, -2.0, 0.0);
//    glVertex3f( 30000.0, -2.0, 0.0);
//    glVertex3f( 0000.0, 2000.0, 0.0);
//    glEnd();


//    qDebug() << "GeometryRenderengine::paintContent: painting"<< layers.count() << "layers...";
    foreach (Layer* layer, layers)
    {
        if (!layer->on)
            continue;

        foreach (CADitem* item, layer->items)
        {

//            // Exclude all items from painting that do not reach the canvas with their boundingRect
//            QRect paintWindow = painter->window();
//            QVector3D p1 = item->boundingBox.p0;
//            QVector3D p2 = item->boundingBox.p0 + item->boundingBox.a1 + item->boundingBox.a2 + item->boundingBox.a3;
//            if (!paintWindow.contains(QRectF(mapFromScene(p1), mapFromScene(p2)).toRect()))
//                continue;

            // Paint it
            switch (item->getType())
            {
            case CADitem::Point:
                break;
            case CADitem::Line:
                paintLine(layer, (CADline*)item);
                break;
            case CADitem::Polyline:
                paintPolyLine(layer, (CADpolyline*)item);
                break;
            case CADitem::Circle:
                break;
            case CADitem::Arc:
                break;
            case CADitem::Face:
                paintFace(layer, (CAD3Dface*)item);
                break;
            case CADitem::Plane:
                break;
            case CADitem::Box:
                break;
            case CADitem::Cylinder:
                break;
            case CADitem::Sphere:
                break;
            case CADitem::LevelSlab:
                break;
            case CADitem::Wall_loadBearing:
                break;
            case CADitem::Wall_nonLoadBearing:
                break;
            case CADitem::BlockOut:
                break;
            case CADitem::Door:
                break;
            case CADitem::Window:
                break;
            }
        }
        paintContent(layer->subLayers);
    }
}

void GLWidget::paintLine(Layer* layer, CADline* item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
    qreal penWidth = 1.0;
    if (item->widthByLayer)
    {
        penWidth = layer->width / 100.0 * zoomFactor;
    }
    else if (item->widthByBlock)
    {

    }
    else
    {
        penWidth = item->width * zoomFactor;
    }

    // Default width setting
    if (penWidth < 1.0)
        pen.setWidth(1);
    else
        pen.setWidthF(penWidth);

//    painter->setPen(pen);

    //  Crop lines that exceed the paint area (heightOfIntersection to depthOfView)
    QVector3D p1 = item->p1;
    QVector3D p2 = item->p2;
//    qreal h1 = heightInScene(p1);
//    qreal h2 = heightInScene(p2);

//    if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
//        return;

//    if (h1 < height_of_intersection)
//    {
//        // modify p1
//        p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
//    }
//    else if (h2 < height_of_intersection)
//    {
//        // modify p2
//        p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
//    }

//    painter->drawLine(mapFromScene(p1), mapFromScene(p2));
//    qDebug() << "GeometryRenderengine: Painting a line";

    glLineWidth(pen.width());
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    glBegin(GL_LINES);
    glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
    glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());

    glEnd();
}

void GLWidget::paintPolyLine(Layer *layer, CADpolyline *item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
//    painter->setPen(pen);

    QVector3D p1 = QVector3D();
    QVector3D p2 = QVector3D();
    glBegin(GL_LINES);
    foreach (CADpolyline::Vertex vertex, item->vertices)
    {
        if (p1 == p2)
        {
            p1 = vertex.pos;
        }
        else
        {
            p2 = vertex.pos;

            //  Crop lines that exceed the paint area (heightOfIntersection to depthOfView)
//            qreal h1 = heightInScene(p1);
//            qreal h2 = heightInScene(p2);

//            if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
//            {
//                p1 = p2;
//                continue;
//            }

//            if (h1 < height_of_intersection)
//            {
//                // modify p1
//                p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
//            }
//            else if (h2 < height_of_intersection)
//            {
//                // modify p2
//                p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
//            }




            // Points in buffer coords (screenpoints)
//            QPoint sp1 = mapFromScene(p1).toPoint() + this->translationOffset;
//            QPoint sp2 = mapFromScene(p2).toPoint() + this->translationOffset;


            glLineWidth(pen.width());
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());

            glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
            glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());



//            painter->drawLine(mapFromScene(p1), mapFromScene(p2));
            p1 = p2;
        }
    }
    glEnd();

    //    qDebug() << "GeometryRenderengine: Painting a polyline";
}

void GLWidget::paintFace(Layer *layer, CAD3Dface *item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
//    painter->setPen(pen);

//    QVector3D p1 = QVector3D();
//    QVector3D p2 = QVector3D();
    glBegin(GL_POLYGON);
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    foreach (CAD3Dface::Vertex vertex, item->vertices)
    {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());

//        if (p1 == p2)
//        {
//            p1 = vertex.pos;
//        }
//        else
//        {
//            p2 = vertex.pos;

//        }
    }
    glEnd();
}
