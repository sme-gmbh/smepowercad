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
    void slot_selectionCountChanged(int num);
    void slot_setDirectionOfViewInvalid();

private slots:
    void on_pushButtonFloat_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonWireframe_clicked(bool checked);
    void on_pushButtonSolid_clicked(bool checked);

    void on_comboBox_view_currentIndexChanged(const QString &arg1);

    void on_spinBoxDepthOfView_valueChanged(const QString &arg1);
    void on_spinBoxHeightOfIntersection_valueChanged(const QString &arg1);

signals:
    void signal_float();
    void signal_close();
    void signal_wireframe(bool on);
    void signal_solid(bool on);
    void signal_cuttingplane_changed(QString directionOfView);
    void signal_cuttingplane_values_changed(qreal height, qreal depth);
};

#endif // GEOMETRYDISPLAYTITLE_H
