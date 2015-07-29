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

#include "keyframeanimation.h"
#include "ui_keyframeanimation.h"

KeyframeAnimation::KeyframeAnimation(QWidget *parent, ItemDB *itemDB) :
    QDialog(parent),
    ui(new Ui::KeyframeAnimation)
{
    this->itemDB = itemDB;
    this->glWidget = new GLWidget(this, itemDB);
    this->glWidget->setAspectRatio(16.0/9.0);
    this->glWidget->slot_perspective(true);
    ui->setupUi(this);
    QVBoxLayout* layout = new QVBoxLayout(ui->graphicWidget);
    layout->addWidget(this->glWidget);

    connect(&timer, SIGNAL(timeout()), this, SLOT(slot_timer_fired()));
}

KeyframeAnimation::~KeyframeAnimation()
{
    delete ui;
}

void KeyframeAnimation::on_pushButton_animate_clicked()
{
    this->animate();
    this->playAnimation();
}

void KeyframeAnimation::on_pushButton_render_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Keyframe Animation: Render to file"), QString(), "MPEG2 Video File (*.mpg)");
    if (filename.isEmpty())
        return;
    QString renderScript = ui->plainTextEdit_renderControlScript->toPlainText();


    // tbd: Change this to render to file later
    this->animate();
    this->render(filename);
}

void KeyframeAnimation::on_pushButton_addKeyframe_clicked()
{
    int duration = QInputDialog::getInt(this, tr("Keyframe Animation"), tr("Enter duration of frame transition"), 25, 1, 3600);
    int startframe = 0;

    if (this->keyframes.size() > 0)
        startframe = this->keyframes.last().frame_end + 1;

    Keyframe new_keyframe;

    new_keyframe.framecontrol.matrix_modelview = this->glWidget->getMatrix_modelview();
    new_keyframe.framecontrol.matrix_rotation = this->glWidget->getMatrix_rotation();
    new_keyframe.frame_start = startframe;
    new_keyframe.frame_end = startframe + duration;

    this->keyframes.append(new_keyframe);
}

void KeyframeAnimation::animate()
{
    if (this->keyframes.size() < 2)
        return;

    int currentFrame = 0;
    int keyframeIndex = 0;
    Keyframe currentKeyframe;
    Keyframe nextKeyframe;

    this->framecontrols.clear();

    do
    {
        do
        {
            currentKeyframe = this->keyframes.at(0 + keyframeIndex);
            nextKeyframe = this->keyframes.at(1 + keyframeIndex);

            // Do the animation interpolation between keyframes
            FrameControl newFramecontrol;
            qreal weightOfFramecontrol = ((qreal)(currentFrame - currentKeyframe.frame_start) / (qreal)(currentKeyframe.frame_end - currentKeyframe.frame_start));
            // Raised cosine interpolation speed control
            weightOfFramecontrol = ((1.0 - cos(weightOfFramecontrol * PI)) / 2.0) * 0.7 + weightOfFramecontrol * 0.3;

            qreal currentZoomfactor = currentKeyframe.framecontrol.matrix_modelview.data()[0];
            qreal nextZoomfactor = nextKeyframe.framecontrol.matrix_modelview.data()[0];
            qreal interpolatedZoomfactor = 1.0 / ( 1.0 / currentZoomfactor * (1.0 - weightOfFramecontrol)
                                                  +1.0 / nextZoomfactor * weightOfFramecontrol);

            QVector4D currentTranslation = currentKeyframe.framecontrol.matrix_modelview.column(3);
            QVector4D nextTranslation = nextKeyframe.framecontrol.matrix_modelview.column(3);
            // tbd: this interpolation should not be done linearily, due to nonlinearity of zoom interpolation
            QVector4D interpolatedTranslation = currentTranslation * (1.0 - weightOfFramecontrol)
                                                 + nextTranslation * weightOfFramecontrol;

            newFramecontrol.matrix_modelview.scale(interpolatedZoomfactor);
            newFramecontrol.matrix_modelview.setColumn(3, interpolatedTranslation);

            // Convert matrices to quaternions
            QQuaternion currentQuaternion = MAngleCalculations().matrixToQuaternion(currentKeyframe.framecontrol.matrix_rotation);
            QQuaternion nextQuaternion    = MAngleCalculations().matrixToQuaternion(nextKeyframe.framecontrol.matrix_rotation);

            QQuaternion interpolatedQuaternion = QQuaternion::slerp(currentQuaternion, nextQuaternion, weightOfFramecontrol);
            QMatrix4x4 internpolatedRotationMatrix = MAngleCalculations().quaternionToMatrix(interpolatedQuaternion);

            // Interpolate rotation
            newFramecontrol.matrix_rotation = internpolatedRotationMatrix;

            // Interpolate rotation based translation component
            QVector4D currentRotTranslation = currentKeyframe.framecontrol.matrix_rotation.column(3);
            QVector4D nextRotTranslation = nextKeyframe.framecontrol.matrix_rotation.column(3);
            QVector4D interpolatedRotTranslation = currentRotTranslation * (1.0 - weightOfFramecontrol)
                                                    + nextRotTranslation * weightOfFramecontrol;
            newFramecontrol.matrix_rotation.setColumn(3, interpolatedRotTranslation);


            this->framecontrols.append(newFramecontrol);

            qDebug() << "weight" << weightOfFramecontrol;
            qDebug() << "quaternion" << interpolatedQuaternion;
            qDebug() << "rotMatrixCurrent" << currentKeyframe.framecontrol.matrix_rotation;
            qDebug() << "rotMatrixNext" << nextKeyframe.framecontrol.matrix_rotation;
            qDebug() << "rotMatrixInt" << newFramecontrol.matrix_rotation;

            currentFrame++;
        } while(currentFrame < currentKeyframe.frame_end);

        keyframeIndex++;

    }while(keyframeIndex < (this->keyframes.size() - 1));
}

void KeyframeAnimation::playAnimation()
{
    this->animationPlayback_currentFrame = 0;
    timer.start(25);
}

void KeyframeAnimation::slot_timer_fired()
{
    if (this->animationPlayback_currentFrame >= this->framecontrols.size())
    {
        timer.stop();
        return;
    }

    this->glWidget->setMatrices(this->framecontrols.at(this->animationPlayback_currentFrame).matrix_modelview,
                                this->framecontrols.at(this->animationPlayback_currentFrame).matrix_rotation);
    this->animationPlayback_currentFrame++;
}

void KeyframeAnimation::render(QString filename)
{
// Video encoding support is not yet finished by qt

//    QMediaObject mediaObject;

//    this->mediaRecorder = new QMediaRecorder(mediaObject);
//    QVideoEncoderSettings videoSettings;
//    videoSettings.setCodec("video/mpeg2");
//    videoSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
//    videoSettings.setQuality(QMultimedia::HighQuality);
//    videoSettings.setResolution(1920, 1080);
//    videoSettings.setFrameRate(25.0);
//    mediaRecorder.setVideoSettings(videoSettings);
//    mediaRecorder.setOutputLocation(QUrl(filename));
//    mediaRecorder.record(); // start recording

    // So for the moment we use the dirty way of feeding ffmpeg with files

    // Check if file exists
    if (QFile(filename).exists())
        QDir().remove(filename);

    QString tempPath = QDir::tempPath();
    if (!QDir(tempPath + "/smepowercad_tmp").exists())
    {
        if (!QDir(tempPath).mkdir("smepowercad_tmp"))
        {
            QMessageBox::critical(this, tr("Keyframe Animation"), tr("Failed to create temporary directory."));
            return;
        }
    }
    tempPath.append("/smepowercad_tmp/");

    // Do the image rendering an save images to filesystem
    this->animationPlayback_currentFrame = 0;
    while (this->animationPlayback_currentFrame < this->framecontrols.size())
    {
        QImage image(1920, 1080, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);

        // Live render to see progress
        this->glWidget->setMatrices(this->framecontrols.at(this->animationPlayback_currentFrame).matrix_modelview,
                                    this->framecontrols.at(this->animationPlayback_currentFrame).matrix_rotation);

        qApp->processEvents();

        // Render to image
        this->glWidget->render_image(&painter, 0, 0, 1920, 1080,
                                     this->framecontrols.at(this->animationPlayback_currentFrame).matrix_modelview,
                                     this->framecontrols.at(this->animationPlayback_currentFrame).matrix_rotation);

        painter.end();

        // Save image
        image.save(tempPath + QString().sprintf("%06i.png", this->animationPlayback_currentFrame), "PNG", -1);

        this->animationPlayback_currentFrame++;
    }

    // Render video by feeding ffmpeg
    QProcess process_ffmpeg(this);

    QStringList args;
    args << "-i" << tempPath + "%06d.png" << "-c:v" << "mpeg2video" << "-b:v" << "10M" << "-vf" << "fps=25,format=yuv420p";
    args << filename;
    process_ffmpeg.execute("ffmpeg", args);

    QDir(tempPath).removeRecursively();

    QMessageBox::information(this, tr("Keyframe Animation"), tr("Rendering finished."));
}
