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

    //check whether triangle 1 completly lies on one side of the plane in which lies triangle 2
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

    //check whether triangle 2 completly lies on one side of the plane in which lies triangle 1
    if (dist_w0 > TOL && dist_w1 > TOL && dist_w2 > TOL)
        return false;
    if (dist_w0 < -TOL && dist_w1 < -TOL && dist_w2 < -TOL)
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
        QVector3D direction = QVector3D::crossProduct(n, m);

        //projections
        float p_v0 = QVector3D::dotProduct(direction, v0);
        float p_v1 = QVector3D::dotProduct(direction, v1);
        float p_v2 = QVector3D::dotProduct(direction, v2);
        float p_w0 = QVector3D::dotProduct(direction, w0);
        float p_w1 = QVector3D::dotProduct(direction, w1);
        float p_w2 = QVector3D::dotProduct(direction, w2);

        /* compute interval for triangle 1 */
        float a, b, c, x0, x1;
        interval(p_v0, p_v1, p_v2, dist_v0, dist_v1, dist_v2, dist_v0 * dist_v1, dist_v0 * dist_v2, &a, &b, &c, &x0, &x1);

        /* compute interval for triangle 2 */
        float d, e, f, y0, y1;
        interval(p_w0, p_w1, p_w2, dist_w0, dist_w1, dist_w2, dist_w0 * dist_w1, dist_w0 * dist_w2, &d, &e, &f, &y0, &y1);

        float xx, yy, xxyy, tmp;
        xx = x0 * x1;
        yy = y0 * y1;
        xxyy = xx * yy;

        tmp = a * xxyy;
        QList<float> isect1 = QList<float>();
        QList<float> isect2 = QList<float>();
        isect1.append(tmp + b * x1 * yy);
        isect1.append(tmp + c * x0 * yy);

        tmp = d * xxyy;
        isect2.append(tmp + e * xx * y1);
        isect2.append(tmp + f * xx * y0);

        qSort(isect1);
        qSort(isect2);

        if(isect1[1] < isect2[0] || isect2[1] < isect1[0])
            return false;
        return true;
    }

}

void MIntersection::interval(float VV0, float VV1, float VV2, float D0, float D1, float D2, float D0D1, float D0D2, float* A, float* B, float* C, float* X0, float* X1)
{
    if (D0D1 > 0.0f)
    {
        /* here we know that D0D2<=0.0 */
        /* that is D0, D1 are on the same side, D2 on the other or on the plane */
        *A = VV2;
        *B = (VV0 - VV2) * D2;
        *C = (VV1 - VV2) * D2;
        *X0 = D2 - D0;
        *X1 = D2 - D1;
    }
    else if (D0D2 > 0.0f)
    {
        /* here we know that d0d1<=0.0 */
        *A = VV1;
        *B = (VV0 - VV1) * D1;
        *C = (VV2 - VV1) * D1;
        *X0 = D1 - D0;
        *X1 = D1 - D2;
    }
    else if (D1 * D2 > 0.0f || D0 != 0.0f)
    {
        /* here we know that d0d1<=0.0 or that D0!=0.0 */
        *A = VV0;
        *B = (VV1 - VV0) * D0;
        *C = (VV2 - VV0) * D0;
        *X0 = D0 - D1;
        *X1 = D0 - D2;
    }
    else if (D1 != 0.0f)
    {
        *A = VV1;
        *B = (VV0 - VV1) * D1;
        *C = (VV2 - VV1) * D1;
        *X0 = D1 - D0;
        *X1 = D1 - D2;
    }
    else if (D2 != 0.0f)
    {
        *A = VV2;
        *B = (VV0 - VV2) * D2;
        *C = (VV1 - VV2) * D2;
        *X0 = D2 - D0; *X1 = D2 - D1;
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
