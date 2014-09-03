#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>

class Overlay : public QWidget
{
    Q_OBJECT
public:
    explicit Overlay(QWidget *parent = 0);
    ~Overlay();
    void moveCursor(QPoint pos);
    void hideCursor();
    void pickStart();
    void pickEnd();
    bool isPickActive();
    QRect selection();
    Qt::ItemSelectionMode selectionMode();

    enum SnapMode{SnapNo, SnapEndpoint, SnapCenter, SnapBasepoint};
    void snap_enable(bool on);
    void set_snap_mode(SnapMode mode);
    void set_snapPos(QPoint snapPos);

private:
    QPoint mousePos;
    QPoint pickStartPos;
    bool pickActive;
    bool cursorShown;
    SnapMode snapMode;
    QPoint snapPos;

protected:
    void paintEvent(QPaintEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // OVERLAY_H
