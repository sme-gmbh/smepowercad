#ifndef KEYFRAMEANIMATION_H
#define KEYFRAMEANIMATION_H

#include <QDialog>
#include <QMatrix4x4>
#include <QList>
#include <QTimer>
#include <QInputDialog>
#include <QFileDialog>
#include <QVideoEncoderSettings>
#include <QMediaRecorder>
#include <QMediaObject>
#include <QCamera>
#include <QUrl>
#include <QDir>
#include <QPainter>
#include <QImage>
#include <QProcess>

#include "itemdb.h"
#include "glwidget.h"

namespace Ui {
class KeyframeAnimation;
}

class KeyframeAnimation : public QDialog
{
    Q_OBJECT

public:
    explicit KeyframeAnimation(QWidget *parent, ItemDB *itemDB);
    ~KeyframeAnimation();

    class FrameControl
    {
    public:
        QMatrix4x4 matrix_modelview;
        QMatrix4x4 matrix_rotation;
    };

    class Keyframe
    {
    public:
        FrameControl framecontrol;
        int frame_start;
        int frame_end;
    };

private slots:

    void on_pushButton_render_clicked();
    void on_pushButton_addKeyframe_clicked();
    void slot_timer_fired();

    void on_pushButton_animate_clicked();

private:
    Ui::KeyframeAnimation *ui;
    ItemDB* itemDB;
    GLWidget* glWidget;

    QList<Keyframe> keyframes;
    QList<FrameControl> framecontrols;

    QTimer timer;
    int animationPlayback_currentFrame;

//    QMediaRecorder* mediaRecorder;


    void animate();
    void playAnimation();
    void render(QString filename);
};

#endif // KEYFRAMEANIMATION_H
