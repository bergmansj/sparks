#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <QObject>
#include <QString>
#include <QList>
#include <QRect>
#include <QImage>

#include <opencv2/opencv.hpp>

class FaceDetection
{
public:
    FaceDetection(const QString& cascadeName);

    QList<QRect> detectFace(const QImage& image);

private:
    void setHaarCascadeXML(const QString& cascadeName);

    CvHaarClassifierCascade* cascade;
};

#endif // FACEDETECTION_H
