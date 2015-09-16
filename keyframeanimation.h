/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#ifndef KEYFRAMEANIMATION_H
#define KEYFRAMEANIMATION_H

#include <QCamera>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QImage>
#include <QInputDialog>
#include <QList>
#include <QMatrix4x4>
#include <QMediaRecorder>
#include <QMediaObject>
#include <QPainter>
#include <QProcess>
#include <QQuaternion>
#include <QResizeEvent>
#include <QUrl>
#include <QVideoEncoderSettings>
#include <QTimer>

#include "itemdb.h"
#include "glwidget.h"

namespace Ui {
class KeyframeAnimation;
}

class KeyframeAnimation : public QDockWidget
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
