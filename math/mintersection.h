#ifndef MINTERSECTION_H
#define MINTERSECTION_H
#define TOL 10E-8

#include <QVector3D>
#include <QVector2D>

class MIntersection
{
public:
    MIntersection();
    ~MIntersection();

    bool trianglesIntersect(QVector3D v0, QVector3D v1, QVector3D v2, QVector3D w0, QVector3D w1, QVector3D w2);

private:
    void interval(float v0, float v1, float v2, float dist_v0, float dist_v1, float dist_v2, float* x0, float*x1);
    bool edgeAgainstEdge(QVector2D v0, QVector2D v1, QVector2D w0, QVector2D w1);
    bool vertexInTriangle(QVector2D v0, QVector2D w0, QVector2D w1, QVector2D w2);
};

#endif // MINTERSECTION_H
