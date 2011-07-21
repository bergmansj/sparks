#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <QDir>
#include <QSize>
#include <QImage>
#include <QString>

#include "facedetection.h"
#include "recognizer/recognizer.h"
#include "recognizer/eigenfacerecognizer.h"

class FaceRecognition : public QObject
{
    Q_OBJECT

public:
    FaceRecognition(const QString& cascadeName, QObject *parent = 0);
    virtual ~FaceRecognition();
    static const QString IMAGE_EXT;
    static const QString VERSION;
    static const QString APPLICATION;

    void setFaceImageSize(QSize);
    void setTrainFolder(QString);

    void addTrainingImage(QImage image);
    void trainRecognizer();


signals:
    void unlabeledFace(const QImage&);
    void recognizeFaceResult(const QImage&, const QString&);
    void recognizeFaceFailed(const QImage&);

    void newStatus(const QString&);

public slots:
    void labeledFace(const QImage&, const QString&);
    void recognizeFace(const QImage&);

private:
    FaceDetection face_detection;
    QSize face_size;
    QDir train_image_path;

    cognition::EigenfaceRecognizer* recognizer;
};

#endif // FACERECOGNITION_H
