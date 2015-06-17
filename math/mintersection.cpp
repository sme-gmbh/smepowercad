#include "mintersection.h"
#include <QDebug>

MIntersection::MIntersection()
{

}

MIntersection::~MIntersection()
{

}

bool MIntersection::trianglesIntersect(QVector3D v0, QVector3D v1, QVector3D v2, QVector3D w0, QVector3D w1, QVector3D w2)
{
    //compare to: http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf
    //thanks to Tomas Möller

    //plane for triangle 2
    QVector3D m = QVector3D::crossProduct(w1 - w0, w2 - w0).normalized();
    double e = - QVector3D::dotProduct(m, w0);
    double dist_v0 = QVector3D::dotProduct(v0, m) + e;
    double dist_v1 = QVector3D::dotProduct(v1, m) + e;
    double dist_v2 = QVector3D::dotProduct(v2, m) + e;

    //check whether triangle 1 completly lies on one side of the plane in which lies triangle 2
    if (dist_v0 > -TOL && dist_v1 > -TOL && dist_v2 > -TOL)
        return false;
    if (dist_v0 < TOL && dist_v1 < TOL && dist_v2 < TOL)
        return false;

    //plane for triangle 1
    QVector3D n = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();
    double d = - QVector3D::dotProduct(n, v0);
    double dist_w0 = QVector3D::dotProduct(w0, n) + d;
    double dist_w1 = QVector3D::dotProduct(w1, n) + d;
    double dist_w2 = QVector3D::dotProduct(w2, n) + d;

    //check whether triangle 2 completly lies on one side of the plane in which lies triangle 1
    if (dist_w0 > -TOL && dist_w1 > -TOL && dist_w2 > -TOL)
        return false;
    if (dist_w0 < TOL && dist_w1 < TOL && dist_w2 < TOL)
        return false;

    if (abs(dist_v0) < TOL && abs(dist_v1) < TOL && abs(dist_v2) < TOL)
    {
        //*****triangles are coplanar*****

        //project "onto the axis-aligned plane where the areas of the triangles are maximized."
        QVector2D v0_2D, v1_2D, v2_2D, w0_2D, w1_2D, w2_2D;
        QVector3D a = QVector3D(qAbs(n.x()), qAbs(n.y()), qAbs(n.z()));
        if(a.x() > a.y())
        {
            if(a.x() > a.z())
            {
                v0_2D = QVector2D(v0.y(), v0.z());
                v1_2D = QVector2D(v1.y(), v1.z());
                v2_2D = QVector2D(v2.y(), v2.z());
                w0_2D = QVector2D(w0.y(), w0.z());
                w1_2D = QVector2D(w1.y(), w1.z());
                w2_2D = QVector2D(w2.y(), w2.z());
            }
            else
            {
                v0_2D = QVector2D(v0.x(), v0.y());
                v1_2D = QVector2D(v1.x(), v1.y());
                v2_2D = QVector2D(v2.x(), v2.y());
                w0_2D = QVector2D(w0.x(), w0.y());
                w1_2D = QVector2D(w1.x(), w1.y());
                w2_2D = QVector2D(w2.x(), w2.y());
            }
        }
        else
        {
            if(a.z() > a.y())
            {
                v0_2D = QVector2D(v0.x(), v0.y());
                v1_2D = QVector2D(v1.x(), v1.y());
                v2_2D = QVector2D(v2.x(), v2.y());
                w0_2D = QVector2D(w0.x(), w0.y());
                w1_2D = QVector2D(w1.x(), w1.y());
                w2_2D = QVector2D(w2.x(), w2.y());
            }
            else
            {
                v0_2D = QVector2D(v0.x(), v0.z());
                v1_2D = QVector2D(v1.x(), v1.z());
                v2_2D = QVector2D(v2.x(), v2.z());
                w0_2D = QVector2D(w0.x(), w0.z());
                w1_2D = QVector2D(w1.x(), w1.z());
                w2_2D = QVector2D(w2.x(), w2.z());
            }
        }

        //test all edges against all edges
        if (edgeAgainstEdge(v0_2D, v1_2D, w0_2D, w1_2D))
            return true;
        if (edgeAgainstEdge(v0_2D, v1_2D, w1_2D, w2_2D))
            return true;
        if (edgeAgainstEdge(v0_2D, v1_2D, w0_2D, w2_2D))
            return true;
        if (edgeAgainstEdge(v1_2D, v2_2D, w0_2D, w1_2D))
            return true;
        if (edgeAgainstEdge(v1_2D, v2_2D, w1_2D, w2_2D))
            return true;
        if (edgeAgainstEdge(v1_2D, v2_2D, w0_2D, w2_2D))
            return true;
        if (edgeAgainstEdge(v0_2D, v2_2D, w0_2D, w1_2D))
            return true;
        if (edgeAgainstEdge(v0_2D, v2_2D, w1_2D, w2_2D))
            return true;
        if (edgeAgainstEdge(v0_2D, v2_2D, w0_2D, w2_2D))
            return true;

        //test if one triangle lies within another
        if (vertexInTriangle(v0_2D, w0_2D, w1_2D, w2_2D))
            return true;
        if (vertexInTriangle(w0_2D, v0_2D, v1_2D, v2_2D))
            return true;

        return false;
    }
    else
    {
        //*****triangles are not coplanar*****

        //compute intersection line
        QVector3D direction = QVector3D::crossProduct(n, m).normalized();

        //projections
        float p_v0 = QVector3D::dotProduct(direction, v0);
        float p_v1 = QVector3D::dotProduct(direction, v1);
        float p_v2 = QVector3D::dotProduct(direction, v2);
        float p_w0 = QVector3D::dotProduct(direction, w0);
        float p_w1 = QVector3D::dotProduct(direction, w1);
        float p_w2 = QVector3D::dotProduct(direction, w2);

        float s1 = 0.0;
        float s2 = 0.0;
        float t1 = 0.0;
        float t2 = 0.0;

        //v0 and v1 on same side
        if(dist_v0 * dist_v1 > TOL)
        {
            s1 = p_v0 + (p_v2 - p_v0) * dist_v0 / (dist_v0 - dist_v2);
            s2 = p_v1 + (p_v2 - p_v1) * dist_v1 / (dist_v1 - dist_v2);
        }
        //v0 and v2 on same side
        else if(dist_v0 * dist_v2 > TOL)
        {
            s1 = p_v0 + (p_v1 - p_v0) * dist_v0 / (dist_v0 - dist_v1);
            s2 = p_v2 + (p_v1 - p_v2) * dist_v2 / (dist_v2 - dist_v1);
        }
        //v1 and v2 on same side
        else //if(dist_v1 * dist_v2 > TOL)
        {
            s1 = p_v1 + (p_v0 - p_v1) * dist_v1 / (dist_v1 - dist_v0);
            s2 = p_v2 + (p_v0 - p_v2) * dist_v2 / (dist_v2 - dist_v0);
        }

        //*********************************************************************

        //w0 and w1 on same side
        if(dist_w0 * dist_w1 > TOL)
        {
            t1 = p_w0 + (p_w2 - p_w0) * dist_w0 / (dist_w0 - dist_w2);
            t2 = p_w1 + (p_w2 - p_w1) * dist_w1 / (dist_w1 - dist_w2);
        }
        //w0 and w2 on same side
        else if(dist_w0 * dist_w2 > TOL)
        {
            t1 = p_w0 + (p_w1 - p_w0) * dist_w0 / (dist_w0 - dist_w1);
            t2 = p_w2 + (p_w1 - p_w2) * dist_w2 / (dist_w2 - dist_w1);
        }
        //w1 and w2 on same side
        else //if(dist_w1 * dist_w2 > TOL)
        {
            t1 = p_w1 + (p_w0 - p_w1) * dist_w1 / (dist_w1 - dist_w0);
            t2 = p_w2 + (p_w0 - p_w2) * dist_w2 / (dist_w2 - dist_w0);
        }

        //sort interval limits
        float i1, i2;
        if(t1 < t2)
        {
            i1 = t1;
            i2 = t2;
        }
        else
        {
            i1 = t2;
            i2 = t1;
        }

        float j1, j2;
        if(s1 < s2)
        {
            j1 = s1;
            j2 = s2;
        }
        else
        {
            j1 = s2;
            j2 = s1;
        }

        //check for intersection
        if((i1 < j1 + 1) && (i2 < j1 + 1))
            return false;
        if((i1 > j2 - 1) && (i2 > j2 - 1))
            return false;

        //some debug output, generates XML, that can be loaded into PowerCAD
        float norm_n_squared = QVector3D::dotProduct(n, n);
        float norm_m_squared = QVector3D::dotProduct(m, m);
        float n_dot_m = QVector3D::dotProduct(n, m);
        QVector3D aufpunkt = (-d * norm_m_squared + e * n_dot_m) / (norm_n_squared * norm_m_squared - n_dot_m * n_dot_m) * n +
                (-e * norm_n_squared + d * n_dot_m) / (norm_n_squared * norm_m_squared - n_dot_m * n_dot_m) * m;

        qDebug() << "<I54 "
                 << "Position_x1=" << '"' << (aufpunkt - 10000*direction).x()<< '"'
                 << "Position_y1=" << '"' << (aufpunkt - 10000*direction).y()<< '"'
                 << "Position_z1=" << '"' << (aufpunkt - 10000*direction).z()<< '"'
                 << "Position_x2=" << '"' << (aufpunkt + 10000*direction).x()<< '"'
                 << "Position_y2=" << '"' << (aufpunkt + 10000*direction).y()<< '"'
                 << "Position_z2=" << '"' << (aufpunkt + 10000*direction).z()<< '"'
                 << "Width=" << '"' << 1 << '"'
                 << "/>" ;

        qDebug() << "<I60 "
                 << "Position_x=" << '"' << (aufpunkt + s1*direction).x()<< '"'
                 << "Position_y=" << '"' << (aufpunkt + s1*direction).y()<< '"'
                 << "Position_z=" << '"' << (aufpunkt + s1*direction).z()<< '"'
                 << "r=" << '"' << 10 << '"'
                 << "/>" ;
        qDebug() << "<I60 "
                 << "Position_x=" << '"' << (aufpunkt + s2*direction).x()<< '"'
                 << "Position_y=" << '"' << (aufpunkt + s2*direction).y()<< '"'
                 << "Position_z=" << '"' << (aufpunkt + s2*direction).z()<< '"'
                 << "r=" << '"' << 10 << '"'
                 << "/>" ;
        qDebug() << "<I60 "
                 << "Position_x=" << '"' << (aufpunkt + t1*direction).x()<< '"'
                 << "Position_y=" << '"' << (aufpunkt + t1*direction).y()<< '"'
                 << "Position_z=" << '"' << (aufpunkt + t1*direction).z()<< '"'
                 << "r=" << '"' << 10 << '"'
                 << "/>" ;
        qDebug() << "<I60 "
                 << "Position_x=" << '"' << (aufpunkt + t2*direction).x()<< '"'
                 << "Position_y=" << '"' << (aufpunkt + t2*direction).y()<< '"'
                 << "Position_z=" << '"' << (aufpunkt + t2*direction).z()<< '"'
                 << "r=" << '"' << 10 << '"'
                 << "/>" ;

        qDebug() << "Parameter:";
        qDebug() << s1 << s2;
        qDebug() << t1 << t2;

        return true;
    }

}


bool MIntersection::trianglesIntersect(MTriangle t1, MTriangle t2)
{
    return trianglesIntersect(t1.getV0(), t1.getV1(), t1.getV2(), t2.getV0(), t2.getV1(), t2.getV2());
}

bool MIntersection::edgeAgainstEdge(QVector2D v0, QVector2D v1, QVector2D w0, QVector2D w1)
{
    float a = (v1 - v0).x();
    float b = (w1 - w0).x();
    float c = (v1 - v0).y();
    float d = (w1 - w0).y();
    // linear system is not solvable
    if(qAbs(a*d - b*c) < TOL)
        return false;
    // solve linear system
    float k = (v0 - w0).x();
    float l = (v0 - w0).y();
    float mu = (l - c*k / a) / (d - c * b / a);
    float lambda = (k - b* mu) / a;

    if(0.0 < lambda && lambda < 1.0 && 0.0 < mu && mu < 1.0)
        return true;
    return false;

}

bool MIntersection::vertexInTriangle(QVector2D v0, QVector2D w0, QVector2D w1, QVector2D w2)
{
    float a = (w1 - w0).x();
    float b = (w2 - w0).x();
    float c = (w1 - w0).y();
    float d = (w2 - w0).y();
    // linear system is not solvable
    if(qAbs(a*d - b*c) < TOL)
        return false;
    // solve linear system
    float k = (v0 - w0).x();
    float l = (v0 - w0).y();
    float t = (l - c*k / a) / (d - c * b / a);
    float s = (k - b* t) / a;

    if(0.0 <= t && t <= 1.0 && 0.0 <= s && s <= 1.0 && s + t <= 1.0)
        return true;
    return false;
}
