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
    QVector3D m = QVector3D::crossProduct(w1 - w0, w2 - w0);
    double e = - QVector3D::dotProduct(m, w0);
    double dist_v0 = QVector3D::dotProduct(v0, m) + e;
    double dist_v1 = QVector3D::dotProduct(v1, m) + e;
    double dist_v2 = QVector3D::dotProduct(v2, m) + e;
    //Triangle 1 completly lies on one side of the plane in which lies Triangle 2
    if (dist_v0 > TOL && dist_v1 > TOL && dist_v2 > TOL)
        return false;
    if (dist_v0 < -TOL && dist_v1 < -TOL && dist_v2 < -TOL)
        return false;

    //plane for triangle 1
    QVector3D n = QVector3D::crossProduct(v1 - v0, v2 - v0);
    double d = - QVector3D::dotProduct(n, v0);
    double dist_w0 = QVector3D::dotProduct(w0, n) + d;
    double dist_w1 = QVector3D::dotProduct(w1, n) + d;
    double dist_w2 = QVector3D::dotProduct(w2, n) + d;
    //Triangle 2 completly lies on one side of the plane in which lies Triangle 1
    if (dist_w0 > TOL && dist_w1 > TOL && dist_w2 > TOL)
        return false;
    if (dist_w0 < -TOL && dist_w1 < -TOL && dist_w2 < -TOL)
        return false;

    if (abs(dist_v0) < TOL && abs(dist_v1) < TOL && abs(dist_v2) < TOL)
    {
        //*****triangles are coplanar*****
        qDebug() << "coplanar";
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
        qDebug() << "not coplanar";
        //compute intersection line
        QVector3D direction = QVector3D::crossProduct(n, m);

        //projections
        float p_v0 = QVector3D::dotProduct(direction, v0);
        float p_v1 = QVector3D::dotProduct(direction, v1);
        float p_v2 = QVector3D::dotProduct(direction, v2);
        float p_w0 = QVector3D::dotProduct(direction, w0);
        float p_w1 = QVector3D::dotProduct(direction, w1);
        float p_w2 = QVector3D::dotProduct(direction, w2);

        //calculate intervals on intersection line
        float x0, x1;
        interval(p_v0, p_v1, p_v2, dist_v0, dist_v1, dist_v2, &x0, &x1);
        float y0, y1;
        interval(p_w0, p_w1, p_w2, dist_w0, dist_w1, dist_w2, &y0, &y1);
        //qDebug() << "[" << x0 << x1 << "],[" << y0 << y1 << "]";
        if(x1 < y0)
            return false;
        if(y1 < x0)
            return false;
        return true;
    }

}

void MIntersection::interval(float v0, float v1, float v2, float dist_v0, float dist_v1, float dist_v2, float* x0, float* x1)
{
    //v0, v1 on the same side
    if (dist_v0 * dist_v1 >= 0.0)
    {
        *x0 = v0 + (v2 - v0) * dist_v0 / ( dist_v0 - dist_v2);
        *x1 = v1 + (v2 - v1) * dist_v1 / ( dist_v1 - dist_v2);
    }
    //v0, v2 on the same side
    else if (dist_v0 * dist_v2 >= 0.0f)
    {
        *x0 = v0 + (v1 - v0) * dist_v0 / ( dist_v0 - dist_v1);
        *x1 = v2 + (v1 - v2) * dist_v2 / ( dist_v2 - dist_v1);
    }
    //v1, v2 on the same side or v0 not in plane
    else if (dist_v1 * dist_v2 >= 0.0f)
    {
        *x0 = v1 + (v0 - v1) * dist_v1 / ( dist_v1 - dist_v0);
        *x1 = v2 + (v0 - v2) * dist_v2 / ( dist_v2 - dist_v0);
    }
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
