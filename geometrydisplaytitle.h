#ifndef GEOMETRYDISPLAYTITLE_H
#define GEOMETRYDISPLAYTITLE_H

#include <QWidget>

namespace Ui {
class GeometryDisplayTitle;
}

class GeometryDisplayTitle : public QWidget
{
    Q_OBJECT

public:
    explicit GeometryDisplayTitle(QWidget *parent = 0);
    ~GeometryDisplayTitle();

private:
    Ui::GeometryDisplayTitle *ui;
};

#endif // GEOMETRYDISPLAYTITLE_H
