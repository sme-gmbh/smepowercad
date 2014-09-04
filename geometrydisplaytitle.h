#ifndef GEOMETRYDISPLAYTITLE_H
#define GEOMETRYDISPLAYTITLE_H

#include <QWidget>
#include <QVector3D>

namespace Ui {
class GeometryDisplayTitle;
}

class GeometryDisplayTitle : public QWidget
{
    Q_OBJECT

public:
    explicit GeometryDisplayTitle(QWidget *parent = 0);
    ~GeometryDisplayTitle();

    void hideButtons();

private:
    Ui::GeometryDisplayTitle *ui;

public slots:
    void slot_sceneCoordinatesChanged(QVector3D coords);

private slots:
    void on_pushButtonFloat_clicked();
    void on_pushButtonClose_clicked();

signals:
    void signal_float();
    void signal_close();
};

#endif // GEOMETRYDISPLAYTITLE_H
